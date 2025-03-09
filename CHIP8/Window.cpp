#include "Window.h"

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

Window::Window()
{
	SDL_Init(SDL_INIT_VIDEO);
	_window = SDL_CreateWindow("CHIP8", 1024, 768, SDL_WINDOW_RESIZABLE);
	_renderer = SDL_CreateRenderer(_window, nullptr);

	_displayTx = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 640, 320);
	SDL_SetRenderTarget(_renderer, _displayTx);
	SDL_SetRenderScale(_renderer, 10.0f, 10.0f);
	SDL_SetRenderTarget(_renderer, nullptr);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplSDL3_InitForSDLRenderer(_window, _renderer);
	ImGui_ImplSDLRenderer3_Init(_renderer);
}

Window::~Window()
{
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyTexture(_displayTx);
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

void Window::BeginDraw()
{
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
}

void Window::EndDraw()
{
	ImGui::Render();
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(_renderer);
	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), _renderer);
	SDL_RenderPresent(_renderer);
}

void Window::DrawTools()
{
	ImGui::Begin("Tools");
	if (ImGui::Button("Registers"))
		ShowRegisters = true;
	ImGui::End();
}

void Window::DrawDisplay(const std::array<bool, 64 * 32>& display)
{
	SDL_SetRenderTarget(_renderer, _displayTx);
	for (size_t y{}; y < 32; y++)
	{
		for (size_t x{}; x < 64; x++)
		{
			if (display[(y * 64) + x])
				SDL_SetRenderDrawColor(_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
			else
				SDL_SetRenderDrawColor(_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
			
			SDL_RenderPoint(_renderer, x, y);
		}
	}
	SDL_SetRenderTarget(_renderer, nullptr);

	ImGui::Begin("Display");
	ImGui::Image((ImTextureID)_displayTx, ImVec2(_displayTx->w, _displayTx->h));
	ImGui::End();
}

void Window::DrawRegisters(uint16_t pc, uint16_t i, uint16_t sp, const std::array<uint8_t, 16>& v)
{
	ImGui::Begin("Registers", &ShowRegisters);
	ImGui::SeparatorText("Special");
	ImGui::Text("PC: #%X", pc);
	ImGui::Text("I: #%X", i);
	ImGui::Text("SP: #%X", sp);
	ImGui::SeparatorText("General-purpose");
	for (size_t i = 0; i < v.size(); i++)
		ImGui::Text("V%X: %d", i, v[i]);
	ImGui::End();
}

bool Window::GetIsOpen()
{
	return _isOpen;
}

char Window::GetLastKeyPressed()
{
	return _lastKeyPressed;
}