#include "Window.h"

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

Window::Window()
{
	SDL_Init(SDL_INIT_VIDEO);
	_window = SDL_CreateWindow("CHIP8", 1024, 768, SDL_WINDOW_RESIZABLE);
	_renderer = SDL_CreateRenderer(_window, nullptr);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsLight();
	ImGui_ImplSDL3_InitForSDLRenderer(_window, _renderer);
	ImGui_ImplSDLRenderer3_Init(_renderer);
}

Window::~Window()
{
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

void Window::PollEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL3_ProcessEvent(&event);
		switch (event.type)
		{
			case SDL_EVENT_QUIT: _isOpen = false; break;
			case SDL_EVENT_TEXT_INPUT: _lastKeyPressed = *event.text.text; break;
		}
	}
}

void Window::Redraw()
{
	ImGui::Render();
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(_renderer);
	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), _renderer);
	SDL_RenderPresent(_renderer);
}

bool Window::GetIsOpen()
{
	return _isOpen;
}

char Window::GetLastKeyPressed()
{
	return _lastKeyPressed;
}