#include "Maze.h"

Maze::Maze(irrklang::ISoundEngine * engine)
	:keyStoneModel("Models/Cube/cube.obj"),
	keystoneSound("Sounds/keystoneSound.wav", false, engine),
	exitModelOpen("Models/Exit/GateOpen.obj"),
	exitModelClosed("Models/Exit/GateClosed.obj")
{
	this->imageData = nullptr;
	this->path = "";
	this->width = 0;
	this->height = 0;
	this->numComponents = 0;
	this->mazeTexture = 0;

	this->mazeTbo = 0;
	this->mazeVbo = 0;
	this->mazeVao = 0;

	//this->keystoneSound.SetVolume(1);

	// Set maze position, rotation and scale
	this->transform.SetPos(glm::vec3(0, 0, 0));
	this->transform.SetRot(glm::vec3(0, 0, 0));
	this->transform.SetScale(glm::vec3(scaleXZ, scaleY, scaleXZ));

	// Load wall & floor texture
	this->LoadTextures();

	this->LoadMaze("MazePNG/mazeColorCoded.png");

	this->exit = this->CreateExit();

	this->nrOfKeystones = 0;
	this->keystonesCapacity = 5; // Init Allocation of the keystone arr
	this->keystones = new Keystone[this->keystonesCapacity];
	this->lastActivatedKeystone = glm::vec3();

	// Create 3 cubes. Each on a separate floor in the maze
	for (int i = 0; i < NR_OF_START_KEYSTONES; i++)
		this->AddKeystone();

	// Set scale of exit
	this->exit.GetTransform()->SetScale(glm::vec3(
		0.11f * this->transform.GetScale().x,
		0.08f * this->transform.GetScale().y,
		0.11f * this->transform.GetScale().z));

#ifdef DEBUG
	// TEST PRINT
	for (int i = 0; i < this->nrOfKeystones; i++)
	{
		std::cout << "Keystone " << i << " on position: ";
		std::cout << "X: " << this->keystones[i].GetTransform()->GetPos().x;
		std::cout << " Z: " << this->keystones[i].GetTransform()->GetPos().z << std::endl;
	}
#endif

	this->InitiateMazeBuffers();

	this->GenerateDrawOrder();
}

Maze::~Maze()
{
	stbi_image_free(imageData);

	glDeleteBuffers(1, &this->mazeTbo);
	glDeleteBuffers(1, &this->mazeVbo);
	glDeleteVertexArrays(1, &this->mazeVao);

	delete[] this->keystones;
}

int Maze::GetMazeHeight()
{
	return this->height;
}

int Maze::GetMazeWidth()
{
	return this->width;
}

glm::vec3 Maze::GetExitWorldPos()
{
	return this->exit.GetTransform()->GetPos();
}

Exit * Maze::GetExit()
{
	return &this->exit;
}

Transform * Maze::GetTransform()
{
	return &this->transform;
}

glm::vec2 * Maze::GetDrawOrder()
{
	return this->drawOrder;
}

glm::vec3 Maze::TransformToMazeCoords(glm::vec3 pos)
{
	float newX = pos.x;
	float newZ = pos.z;

	// NOT NEEDED Transform world coords to texture coords. ( 1 pixel on texture corresponds to 1.0, origo is (0, 0) for both spaces

	// The maze can be translated
	newX -= this->GetTransform()->GetPos().x;
	newZ -= this->GetTransform()->GetPos().z;

	// The maze can be scaled
	newX /= this->GetTransform()->GetScale().x;
	newZ /= this->GetTransform()->GetScale().z;

	// The walls have a offset, while the maze's center is in the origin (0,0)
	float pixelOffset = 0.5f;
	newX += (this->GetMazeWidth() / 2) + pixelOffset;
	newZ += (this->GetMazeHeight() / 2) + pixelOffset;

	pos.x = newX;
	pos.z = newZ;

	return pos;
}

glm::vec3 Maze::TransformToWorldCoords(glm::vec3 pos)
{
	float newX = pos.x;
	float newZ = pos.z;

	// NOT NEEDED Transform world coords to texture coords. ( 1 pixel on texture corresponds to 1.0, origo is (0, 0) for both spaces

	// The walls have a offset, while the maze's center is in the origin (0,0)
	newX -= (this->GetMazeWidth() / 2);
	newZ -= (this->GetMazeHeight() / 2);

	// The maze can be scaled
	newX *= this->GetTransform()->GetScale().x;
	newZ *= this->GetTransform()->GetScale().z;

	// The maze can be translated
	newX += this->GetTransform()->GetPos().x;
	newZ += this->GetTransform()->GetPos().z;
	
	pos.x = newX;
	pos.z = newZ;

	return pos;
}

unsigned int Maze::GetTileCount()
{
	return (1 + 2 * DRAWDISTANCE)*(1 + 2 * DRAWDISTANCE);
}

Transform * Maze::GetKeystoneTransform(unsigned int index)
{
	return this->keystones[index].GetTransform();
}


int Maze::GetNrOfKeystones()
{
	return this->nrOfKeystones;
}

bool Maze::IsExitOpen()
{
	return this->isExitOpen;
}

// Input in world coords
bool Maze::IsWallAtWorld(float x, float y)
{
	bool isAWall = true;

	glm::vec3 transformed = this->TransformToMazeCoords(glm::vec3(x, 0.0f, y));

	transformed.x = int(transformed.x);
	transformed.z = int(transformed.z);

	if (!this->IsExitOpen() && transformed.x == this->exit.GetExitUVPos().x && transformed.z == this->exit.GetExitUVPos().y)
		return true;

	glm::vec3 pixel = readPixel(transformed.x, transformed.z);
	
	if (pixel == glm::vec3(0.0f, 0.0f, 0.0f))
	{
		isAWall = false;
	}
	
	return isAWall;
}

Wall Maze::GetWallType(float x, float y)
{
	Wall type = NO_WALL;
	glm::vec3 transformed = this->TransformToMazeCoords(glm::vec3(x, 0.0f, y));
	glm::vec3 pixel = readPixel(transformed.x, transformed.z);

	if (pixel == glm::vec3(100.0f, 0.0f, 255.0f))
	{
		type = WALL_UP;
	}
	else if (pixel == glm::vec3(100.0f, 255.0f, 0.0f))
	{
		type = WALL_DOWN;
	}
	else if (pixel == glm::vec3(100.0f, 0.0f, 0.0f))
	{
		type = WALL_LEFT;
	}
	else if (pixel == glm::vec3(100.0f, 255.0f, 255.0f))
	{
		type = WALL_RIGHT;
	}
	else if (pixel == glm::vec3(200.0f, 0.0f, 255.0f))
	{
		type = WALL_CORNER_RIGHT_UP;
	}
	else if (pixel == glm::vec3(200.0f, 255.0f, 0.0f))
	{
		type = WALL_CORNER_LEFT_UP;
	}
	else if (pixel == glm::vec3(200.0f, 0.0f, 0.0f))
	{
		type = WALL_CORNER_LEFT_DOWN;
	}
	else if (pixel == glm::vec3(200.0f, 255.0f, 255.0f))
	{
		type = WALL_CORNER_RIGHT_DOWN;
	}
	else if (pixel == glm::vec3(255.0f, 0.0f, 255.0f))
	{
		type = WALL_END_UP;
	}
	else if (pixel == glm::vec3(255.0f, 255.0f, 0.0f))
	{
		type = WALL_END_RIGHT;
	}
	else if (pixel == glm::vec3(255.0f, 0.0f, 0.0f))
	{
		type = WALL_END_DOWN;
	}
	else if (pixel == glm::vec3(255.0f, 255.0f, 255.0f))
	{
		type = WALL_END_LEFT;
	}
	else if (pixel == glm::vec3(50.0f, 0.0f, 0.0f))
	{
		type = WALL_VERTICAL;
	}
	else if (pixel == glm::vec3(50.0f, 255.0f, 255.0f))
	{
		type = WALL_HORIZONTAL;
	}

	return type;
}

void Maze::BindTexture(unsigned int textureUnit)
{
	if (textureUnit >= 0 && textureUnit <= 31)
	{
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(GL_TEXTURE_2D, this->mazeTexture);
	}
	else
	{
		std::cout << "[ERROR] Texture could not be bound. Unit not in range[0-31]" << std::endl;
	}
}

void Maze::LoadMaze(const std::string & fileName)
{
	this->path = fileName;

	this->imageData = stbi_load(fileName.c_str(), &this->width, &this->height, &this->numComponents, 3);

	if (this->imageData == NULL)
		std::cerr << "Loading failed for texture: " << fileName << std::endl;

	glGenTextures(1, &this->mazeTexture);
	glBindTexture(GL_TEXTURE_2D, this->mazeTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Skickar texturen till GPU'n
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->imageData);
}

Exit Maze::CreateExit()
{
	// Find the exit
	ExitPosDir a = this->FindExit();
	glm::vec3 exitWorldPos = this->TransformToWorldCoords(glm::vec3(a.uvPos.x, 0, a.uvPos.y));
	glm::vec3 exitDir = this->TransformToWorldCoords(glm::vec3(a.uvDir.x, 0.0, a.uvDir.y)) - exitWorldPos;
	exitDir.z *= -1;

	cout << "X: " << exitWorldPos.x << "Y: " << exitWorldPos.y << "Z: " << exitWorldPos.z << endl;

	Exit exit = Exit(&this->exitModelOpen, &this->exitModelClosed, exitWorldPos, exitDir, a.uvPos);
	exit.GetTransform()->GetScale() = glm::vec3(this->scaleXZ, this->scaleY, this->scaleXZ);
	// Create exit
	return exit;
}

void Maze::DrawMazeToBuffer()
{
	// Skip the fragment shader
	glEnable(GL_RASTERIZER_DISCARD);

	glBindVertexArray(this->mazeVao);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, this->mazeTbo);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, this->mazeVbo);

	// Perform transform feedback
	glBeginTransformFeedback(GL_TRIANGLES);
	glDrawArrays(GL_POINTS, 0, (1 + 2 * DRAWDISTANCE)*(1 + 2 * DRAWDISTANCE));
	glEndTransformFeedback();

	// Enable the fragment shader again
	glDisable(GL_RASTERIZER_DISCARD);

	// Something ...
	glFlush();

	// Memory barrier
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);
	glBindVertexArray(0);
}

void Maze::DrawMaze()
{
	glBindVertexArray(this->mazeVao);
	
	glDrawTransformFeedback(GL_PATCHES, this->mazeTbo);
	
	// Memory barrier
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);
	glBindVertexArray(0);
}

void Maze::DrawMazeShadows()
{
	glBindVertexArray(this->mazeVao);

	glDrawTransformFeedback(GL_TRIANGLES, this->mazeTbo);

	// Memory barrier
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);
	glBindVertexArray(0);
}

void Maze::DrawKeystone(unsigned int index, Shader * shader)
{
	this->keystones[index].Draw(&this->keyStoneModel, shader);
}

bool Maze::ActivateKeystone(glm::vec3 playerPos, Sound * minotaurGrowlSound)
{
	bool activated = false;

	// These vectors are used to calculated the distance between the keystone and the player
	glm::vec2 tempPlayerPos;
	tempPlayerPos.x = playerPos.x;
	tempPlayerPos.y = playerPos.z;
	glm::vec2 tempKeystonePos;

	float distance = 0;

	// Find which keystone was pressed
	for (int i = 0; (i < this->nrOfKeystones) && (activated == false); i++)
	{
		// Fill the tempKeystonePos with the keystone position
		tempKeystonePos.x = this->keystones[i].GetTransform()->GetPos().x;
		tempKeystonePos.y = this->keystones[i].GetTransform()->GetPos().z;

		distance = length(tempPlayerPos - tempKeystonePos);

		// If the keystone is inactive (It hasn't been pressed before)
		if (this->keystones[i].IsActive() == false)
		{
			// Check if the player is close enough to be able to press the keystone
			if (distance < 1.0)
			{
				this->keystones[i].ActivateKeystone();
				activated = true;
#ifdef DEBUG
				std::cout << "Keystone " << i << " Activated!" << std::endl;
#endif
				this->keystoneSound.Play();
				this->keystoneSound.SetPosition(glm::vec3(tempKeystonePos.x, 0.5f, tempKeystonePos.y));
				minotaurGrowlSound->Play();
			}
		}	
	}

	// Check if exit is open
	this->isExitOpen = true;
	for (int i = 0; i < this->nrOfKeystones; i++)
	{
		// if a keystone isn't active. We stop looping and return false
		if (this->keystones[i].IsActive() == false)
		{
			this->isExitOpen = false;
			break;
		}
	}

	return activated;
}

void Maze::UpdateKeystones(float deltaTime)
{
	for (int i = 0; i < this->nrOfKeystones; i++)
	{
		if (this->keystones[i].IsActive() == true)
		{
			if (this->keystones[i].IsTranslatedBack() == false)
			{
				// Declaring some variables to make it look good
				float movementSpeed = this->keystones[i].GetMovementSpeed() * deltaTime;
				glm::vec3 direction = -this->keystones[i].GetDirection();

				// Update the position
				this->keystones[i].GetTransform()->GetPos() += movementSpeed * direction;
				
				this->keystones[i].UpdateTranslationLength(abs(movementSpeed));

				// Check if the keystone is done translating
				if (this->keystones[i].GetTranslationLength() <= 0)
				{
					this->keystones[i].SetIsTranslatedBack(true);
				}
			}	
		}
	}
}

void Maze::ResetKeystones()
{
	this->nrOfKeystones = 0;
	// Create 3 cubes. Each on a separate floor in the maze
	for (int i = 0; i < NR_OF_START_KEYSTONES; i++)
		this->AddKeystone();
}

void Maze::BindMaterial(Shader* shader)
{
	this->floor0Mat->BindMaterialArray(shader, 0);
	this->wall1Mat->BindMaterialArray(shader, 1);
}

void Maze::InitiateMazeBuffers()
{
	// Three walls can be drawn with the same point, hence 18 vertices and not 6
	GLint maxNrOfVertices = 18;

	// mazeVao to draw points
	glGenVertexArrays(1, &this->mazeVao);
	glBindVertexArray(this->mazeVao);

	// Create a buffer to hold the results of the transform feedback process.
	glGenBuffers(1, &this->mazeVbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->mazeVbo);

	// Allocate space (no data)
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(glm::vec3)	 * maxNrOfVertices * (1 + 2 * DRAWDISTANCE)*(1 + 2 * DRAWDISTANCE) +	// Position
		sizeof(glm::vec2)	 * maxNrOfVertices * (1 + 2 * DRAWDISTANCE)*(1 + 2 * DRAWDISTANCE) +	// Texcoords
		sizeof(glm::vec3)	 * maxNrOfVertices * (1 + 2 * DRAWDISTANCE)*(1 + 2 * DRAWDISTANCE) +	// Normals
		sizeof(glm::vec3)	 * maxNrOfVertices * (1 + 2 * DRAWDISTANCE)*(1 + 2 * DRAWDISTANCE) +	// Tangents
		sizeof(float)		 * maxNrOfVertices * (1 + 2 * DRAWDISTANCE)*(1 + 2 * DRAWDISTANCE),	// Type
		NULL,							// no data passed
		GL_DYNAMIC_COPY);

	// Set the output Layout
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3		 * sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(float), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 3		 * sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(float), (const GLvoid*)(sizeof(glm::vec3)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3		 * sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(float), (const GLvoid*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3		 * sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(float), (const GLvoid*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 3		 * sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(float), (const GLvoid*)(3 * sizeof(glm::vec3) + sizeof(glm::vec2)));

	// Create and bind transform feedback object and buffer to write to.
	glGenTransformFeedbacks(1, &this->mazeTbo);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, this->mazeTbo);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, this->mazeVbo);

	glBindVertexArray(0);
}

void Maze::GenerateDrawOrder()
{
	int layer = 0;
	int ID = 0;

	if (layer == 0)
	{
		this->drawOrder[ID].x = 0;
		this->drawOrder[ID].y = 0;
		ID++;
		layer++;
	}

	while (layer <= DRAWDISTANCE)
	{
		for (int y = (-layer); y < (1 + (2 * layer)) - layer; y++)
		{
			for (int x = (-layer); x < (1 + (2 * layer)) - layer; x++)
			{
				if (x == 1 - layer && y > (-layer) && y < (2 * layer) - layer)
					x += (1 + 2 * (layer - 1));
				this->drawOrder[ID].x = x;
				this->drawOrder[ID].y = y;
				ID++;
			}
		}
		layer++;
	}
}

void Maze::LoadTextures()
{
	MaterialHandler& MH = MaterialHandler::GetInstance();

	Texture* wallDiffuse = MH.LoadTexture("Textures/wall0/wall0_diffuse.png", "TextureDiffuse");
	Texture* wallNormal = MH.LoadTexture("Textures/wall0/wall0_normal.png", "TextureNormal");
	Texture* wallAmbient = MH.LoadTexture("Textures/wall0/wall0_ambient.png", "TextureAmbient");
	Texture* wallSpecular = MH.LoadTexture("Textures/wall0/wall0_specular.png", "TextureSpecular");
	Texture* wallHeight = MH.LoadTexture("Textures/wall0/wall0_height.png", "TextureHeight");
	float wallShininess = 12.0;

	
	Texture* floorDiffuse = MH.LoadTexture("Textures/floor0/floor0_diffuse.png", "TextureDiffuse");
	Texture* floorNormal = MH.LoadTexture("Textures/floor0/floor0_normal.png", "TextureNormal");
	Texture* floorAmbient = MH.LoadTexture("Textures/floor0/floor0_ambient.png", "TextureAmbient");
	Texture* floorSpecular = MH.LoadTexture("Textures/floor0/floor0_specular.png", "TextureSpecular");
	Texture* floorHeight = MH.LoadTexture("Textures/floor0/floor0_height.png", "TextureHeight");
	float floorShininess = 12.0;

	

	// Wall0
	this->wall1Mat = MH.AddMaterial(wallDiffuse, wallAmbient, wallSpecular, wallNormal, wallHeight, wallShininess, "wall1");

	// Floor0
	this->floor0Mat = MH.AddMaterial(floorDiffuse, floorAmbient, floorSpecular, floorNormal, floorHeight, floorShininess, "floor0");
}

// Returns a vector with the rgb value of a pixel
glm::vec3 Maze::readPixel(unsigned int x, unsigned int y)
{
	// Check if pixel is outside of the texture
	if (x > this->width - 1 || y > this->height - 1)
	{
		return glm::vec3(-1.0f);
	}

	unsigned char* pixelOffset = this->imageData + (x + this->width * y) * this->numComponents;

	vector<unsigned char> pixel;
	for (int i = 0; i < 3; i++)
	{
		pixel.push_back(pixelOffset[i]);
	}

	return glm::vec3(pixel[0], pixel[1], pixel[2]);
}

ExitPosDir Maze::FindExit()
{
	glm::vec2 exitPos(-1.0);
	glm::vec2 exitDir(-1.0);

	// Find exit
	for (int x= 0; x < this->width; x++)
	{
		// --->
		if (this->readPixel(x, 0) == glm::vec3(0.0))
		{
			exitPos = glm::vec2(x, 0);
			exitDir = glm::vec2(x, 1);
			break;
		}

		// |
		// V
		if (this->readPixel(0, x) == glm::vec3(0.0))
		{
			exitPos = glm::vec2(0, x);
			exitDir = glm::vec2(1, x);
			break;
		}

		// ---> mirror
		if (this->readPixel(x, this->height - 2) == glm::vec3(0.0))
		{
			exitPos = glm::vec2(x, this->height - 2);
			exitDir = glm::vec2(x, this->height - 3);
			break;
		}

		// |
		// V mirror
		if (this->readPixel(this->width - 2, x) == glm::vec3(0.0))
		{
			exitPos = glm::vec2(this->width - 2, x);
			exitDir = glm::vec2(this->width - 3, x);
			break;
		}
	}

	if (exitPos == glm::vec2(-1.0))
	{
		cout << "[ERROR]: Exit not found" << endl;
	}

#ifdef DEBUG
	cout << "Exit World Pos (X,Z): " << exitPos.x << ", " << exitPos.y << endl;
#endif

	ExitPosDir r = {exitPos, exitDir};
	return r;
}

KeystonePosDir Maze::CreateCubePosition()
{
	while (true)
	{
	start:	// This function will restart if the position found isn't a wall AND if it cannot find a adjecent floor

		int randomWidth = rand() % this->width -2;
		int randomHeight = rand() % this->height -2;	

		glm::vec3 randomPosWorld = this->TransformToWorldCoords(glm::vec3(randomWidth, 0, randomHeight));

		// Dont allow keystones to spawn next to the exit
		if(abs(randomPosWorld.x - this->exitWorldPos.x < 3 * this->scaleXZ) &&
			abs(randomPosWorld.z - this->exitWorldPos.z < 3 * this->scaleXZ))
		{
			goto start;
		}
		// Check distance between other cubes (if there is any)
		for (int i = 0; i < this->nrOfKeystones; i++)
		{
			// Dont allow keystones to spawn next to eachother
			if (abs(randomPosWorld.x - this->keystones[i].GetTransform()->GetPos().x) < 5 * this->scaleXZ &&
				abs(randomPosWorld.z - this->keystones[i].GetTransform()->GetPos().z) < 5 * this->scaleXZ)
			{
				// The points are to close to eachother, we need to find a new position
				randomWidth = rand() % this->width -2;
				randomHeight = rand() % this->height -2;
		
				randomPosWorld = this->TransformToWorldCoords(glm::vec3(randomWidth, 0, randomHeight));
		
				// Restart loop
				i = 0;
			}
		}

		glm::vec3 floorColor = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 pixelColor = this->readPixel(randomWidth, randomHeight);
		
		glm::vec3 nearbyFloorPos;
		// Find nearby floor, This is used to place the keystone towards the floor
		nearbyFloorPos = this->FindNearbyFloor(glm::vec2(randomWidth, randomHeight));

		// If a nearbyfloor wasn't found we restart the function, to find another position
		if (nearbyFloorPos != glm::vec3(-1.0f))
		{
			if (pixelColor != floorColor)
			{
				// Transform to world coords
				nearbyFloorPos = this->TransformToWorldCoords(nearbyFloorPos);
				glm::vec3 wallPos = this->TransformToWorldCoords(glm::vec3(randomWidth, 0.5f, randomHeight));

				// Vector from wall to floor
				glm::vec3 direction = normalize(nearbyFloorPos - wallPos);

				// Translate the cube so that its location is in the middle of a wall and a floor
				glm::vec3 finalPosition = wallPos + (direction * float(this->scaleXZ) / 2.0f);

				KeystonePosDir keystonePosDir = { finalPosition, direction };

				return keystonePosDir;
			}
		}	
	}
}

// This functions takes a position in mazeCoords, and returns a nearby floor in maze coords
glm::vec3 Maze::FindNearbyFloor(glm::vec2 wallPos)
{
	glm::vec3 floorPixel;

	// Read NEARby pixels
	// 
	//	    _0_	
	//	 3 | . | 1
	//	   |___|
	//	  	 2   

	glm::vec3 wallPosAtWorld;

	// Read 0 (up)
	floorPixel = this->readPixel(wallPos.x, wallPos.y + 1);	
	if (floorPixel != glm::vec3(-1.0f))
	{
		wallPosAtWorld = this->TransformToWorldCoords(glm::vec3(wallPos.x, 0.0f, wallPos.y + 1));

		if (this->IsWallAtWorld(wallPosAtWorld.x, wallPosAtWorld.z) == false)
		{
			if (floorPixel == glm::vec3(0.0f, 0.0f, 0.0f))
			{
				return glm::vec3(wallPos.x, 0.5f, wallPos.y + 1);
			}
		}
	}
	

	// Read 2 (down)
	floorPixel = this->readPixel(wallPos.x, wallPos.y - 1);
	if (floorPixel != glm::vec3(-1.0f))
	{
		wallPosAtWorld = this->TransformToWorldCoords(glm::vec3(wallPos.x, 0.0f, wallPos.y - 1));
		if (this->IsWallAtWorld(wallPosAtWorld.x, wallPosAtWorld.z) == false)
		{
			if (floorPixel == glm::vec3(0.0f, 0.0f, 0.0f))
			{
				return glm::vec3(wallPos.x, 0.5f, wallPos.y - 1);
			}
		}
	}

	// Read 1 (right)
	floorPixel = this->readPixel(wallPos.x + 1, wallPos.y);
	if (floorPixel != glm::vec3(-1.0f))
	{
		wallPosAtWorld = this->TransformToWorldCoords(glm::vec3(wallPos.x + 1, 0.0f, wallPos.y));
		if (this->IsWallAtWorld(wallPosAtWorld.x, wallPosAtWorld.z) == false)
		{
			if (floorPixel == glm::vec3(0.0f, 0.0f, 0.0f))
			{
				return glm::vec3(wallPos.x + 1, 0.5f, wallPos.y);
			}
		}
	}

	// Read 3 (left)
	floorPixel = this->readPixel(wallPos.x -1, wallPos.y);
	if (floorPixel != glm::vec3(-1.0f))
	{
		wallPosAtWorld = this->TransformToWorldCoords(glm::vec3(wallPos.x - 1, 0.0f, wallPos.y));
		if (this->IsWallAtWorld(wallPosAtWorld.x, wallPosAtWorld.z) == false)
		{
			if (floorPixel == glm::vec3(0.0f, 0.0f, 0.0f))
			{
				return glm::vec3(wallPos.x - 1, 0.5f, wallPos.y);
			}
		}
	}

	// If a nearbyFloor wasn't found
	return glm::vec3(-1.0f);
}

void Maze::AddKeystone()
{
	// Check for room
	if (this->nrOfKeystones >= this->keystonesCapacity)
	{
		// Expand the array
		this->keystonesCapacity += 10;
		Keystone * tmp = new Keystone[this->keystonesCapacity];

		for (int i = 0; i < this->nrOfKeystones; i++)
		{
			tmp[i] = this->keystones[i];
		}

		delete[] this->keystones;
		this->keystones = tmp;
	}

	// Add a keystone
	this->keystones[this->nrOfKeystones] = Keystone(&this->CreateCubePosition(), this->scaleXZ);
	this->nrOfKeystones++;
}
