#ifndef DISPLAY_H
#define DISPLAY_H
#include <glew\glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <glm/gtc/type_ptr.hpp>
using namespace std;

const int SCREENWIDTH = 800;
const int SCREENHEIGHT = 600;

class Display
{
public:
	Display();

	void SwapBuffers(int width, int height);
	bool IsWindowClosed();
	void SetTitle(string newTitle);
	GLFWwindow* GetWindow();

	virtual ~Display();

private:
	GLFWwindow *window;

};
#endif //DISPLAY_H*/