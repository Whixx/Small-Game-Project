#ifndef DISPLAY_H
#define DISPLAY_H
#include <glew\glew.h>
#include <GLFW/glfw3.h>
#include <string>
using namespace std;

class Display
{
public:
	Display(int width, int height);

	void SwapBuffers(int width, int height);
	bool IsWindowClosed();
	void SetTitle(string newTitle);

	virtual ~Display();

	GLFWwindow *getWindow();
private:
	GLFWwindow *window;

};
#endif //DISPLAY_H*/