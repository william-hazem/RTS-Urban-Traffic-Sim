#pragma once

#ifndef SEMAPHORES_H
#define SEMAPHORES_H


typedef enum e_semaphore_id
{
	A, B, C, D
};

typedef enum e_semaphore_color
{
	Red, Yellow, Green, Deadtime
}e_semaphore_color;


typedef enum e_semahore_phase
{
	F1, F2, F3
};

// Task do semaforo
void t_semaphore(void* args)
{
	/// Variaveis internas
	e_semaphore_color color;
	e_semahore_phase  phase;
	e_semaphore_id	  id;
	/// lógica do semaforo
	for (;;)
	{

	}
}


#endif // SEMAPHORES_H