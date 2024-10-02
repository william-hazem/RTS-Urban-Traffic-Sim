#pragma once

#include "task.h"

#ifndef CARS_H
#define CARS_H

typedef enum {N, S, E, W} e_car_direction;
typedef enum {Red, Green, Blue, Gray, COUNTS} e_car_color;

typedef struct {
	int x, y;			// Coordenadas do carro
	int width, height;
	e_car_color color;  // Cor do carro
	int running;		// Indica se o carro está em movimento
	TaskHandle_t handle;
} Car;


void f_car(void* args)
{
	Car* car = (Car*) args;
	float v;

	// Lógica dos carros
	for (;;)
	{
		if (car->running)
		{
			car->x += 2;
			if (car->x > 800) car->x = 0;
		}
		vTaskDelay(32);
	}
	return;
}


void car_create(float x, float y, e_car_color color, TaskHandle_t handle, Car* car)
{
	static int count = 0;
	car->x = x;
	car->y = y;
	car->width = 20;
	car->height = 10;
	car->color = color;
	car->running = 1;
	car->handle = handle;


	char taskname[50] = "";
	sprintf(taskname, "Car %d", count++);

	xTaskCreate(f_car, taskname, 100, (void*)car, 5, NULL);
}

#endif // !CARS_H
