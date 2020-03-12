//
// Created by jack on 20-2-25.
//
#include "window.h"
Escape::Window::Window(const std::string &title, int width, int height) {
    this->title = title;
    this->width = width;
    this->height = height;
}

void Escape::Window::update(float delta) {
    this->delta = delta;
    processInput();
    render();
    postProcess();
}

void Escape::Window::postProcess() {
}

void Escape::Window::processInput() {
}

Escape::Window::~Window() {
}

void Escape::Window::windowResized(int width, int height) {
    this->width = width;
    this->height = height;
}
