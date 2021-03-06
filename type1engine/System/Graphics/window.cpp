#include "window.h"

namespace Engine
{
	namespace Graphics
	{
		void window_size_callback(GLFWwindow *window, int width, int height)
		{
			Window *win = (Window*)glfwGetWindowUserPointer(window);
			win->m_width = width;
			win->m_height = height;
			glViewport(0, 0, win->m_width, win->m_height);
		}

		void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			Window *win = (Window*)glfwGetWindowUserPointer(window);
			if (action != GLFW_RELEASE)
			{
				win->m_keys[key] = true;
			}
			else
			{
				win->m_keys[key] = false;
			}
		}

		void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
		{
			Window *win = (Window*)glfwGetWindowUserPointer(window);
			if (action != GLFW_RELEASE)
			{
				win->m_mouse[button] = true;
			}
			else
			{
				win->m_mouse[button] = false;
			}
		}

		void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
		{
			Window *win = (Window*)glfwGetWindowUserPointer(window);
			win->m_mouseposX = xpos;
			win->m_mouseposY = ypos;
		}


		Window::Window(const char *t, unsigned int w, unsigned int h)
		{
			m_title = t;
			m_width = w;
			m_height = h;
			if (!Init())
			{
				glfwTerminate();
			}

			for (unsigned int i = 0; i < KEY_MAX; i++)
			{
				m_keys[i] = false;
				m_KeysState[i] = false;
				m_KeysTyped[i] = false;
			}

			for (unsigned int i = 0; i < MOUSE_MAX; i++)
			{
				m_mouse[i] = false;
				m_MouseState[i] = false;
				m_MouseClicked[i] = false;
			}

			SoundManager::init();

			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		}

		Window::~Window()
		{
			glfwTerminate();
			FontManager::Clean();
			SoundManager::Clean();
		}

		bool Window::Init()
		{
			if (!glfwInit())
			{
				printf("GLFW initializing Failed!\n");
				return false;
			}
			m_window = glfwCreateWindow(m_width, m_height, m_title, NULL ,NULL);
			if (!m_window)
			{
				printf("Create Window Failed!\n");
				return false;
			}
			glfwMakeContextCurrent(m_window);
			glfwSetWindowUserPointer(m_window, this);
			glfwSetFramebufferSizeCallback(m_window, window_size_callback);
			glfwSetMouseButtonCallback(m_window, mouse_button_callback);
			glfwSetCursorPosCallback(m_window, cursor_position_callback);
			glfwSwapInterval(0);

			const GLubyte* name = glGetString(GL_VENDOR);
			const GLubyte* identifier = glGetString(GL_RENDERER);
			const GLubyte* version = glGetString(GL_VERSION);
			//const GLubyte* gluVersion = gluGetString(GLU_VERSION);
			const GLubyte* glewVersion = glewGetString(GLEW_VERSION);
			//const GLubyte* glExt = glGetString(GL_EXTENSIONS);

			printf("vendor: %s\n", name);
			printf("identifer: %s\n", identifier);
			printf("opengl version: %s\n", version);
			//printf("glu version: %s\n", gluVersion);
			printf("glfw version: %s\n", glfwGetVersionString());
			printf("glew version: %s\n", glewVersion);
			//printf("glExt: %s\n", glExt);

			if (glewInit() != GLEW_OK)
			{
				printf("GLEW initializing Failed!\n");
				return false;
			}

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glfwSetKeyCallback(m_window, key_callback);

			return true;
		}

		void Window::Clear() const
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void Window::Update()
		{
			for (unsigned int i = 0; i < KEY_MAX; i++)
			{
				m_KeysTyped[i] = m_keys[i] && !m_KeysState[i];
			}

			memcpy(m_KeysState, m_keys, KEY_MAX);

			for (unsigned int i = 0; i < MOUSE_MAX; i++)
			{
				m_MouseClicked[i] = m_mouse[i] && !m_MouseState[i];
			}

			memcpy(m_MouseState, m_mouse, MOUSE_MAX);

			GLenum error = glGetError();
			if (error != GL_NO_ERROR)
			{
				std::cout << "OpenGL Error: " << error << "->" <<gluErrorString(error) << std::endl;
			}
			glfwPollEvents();
			glfwSwapBuffers(m_window);
		}

		int Window::IsClosed() const
		{
			return glfwWindowShouldClose(m_window);
		}

		void Window::Close() const
		{
			glfwSetWindowShouldClose(m_window, GLFW_TRUE);
		}

		bool Window::IsKeyPressed(unsigned int key) const
		{
			return m_keys[key];
		}

		bool Window::IsKeyTyped(unsigned int key) const
		{
			return m_KeysTyped[key];
		}

		bool Window::IsMouseButtonPressed(unsigned int button) const
		{
			return m_mouse[button];
		}

		bool Window::IsMouseButtonClicked(unsigned int button) const
		{
			return m_MouseClicked[button];
		}

		void Window::GetMousePosition(double *x, double *y) const
		{
			*x = m_mouseposX;
			*y = m_mouseposY;
		}
	}
}