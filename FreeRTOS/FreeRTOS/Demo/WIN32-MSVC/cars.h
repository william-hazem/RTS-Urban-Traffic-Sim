#pragma once

#include "task.h"
#include "SDL_pixels.h"
#include "mapping.h"

#ifndef CARS_H
#define CARS_H

// Defini��es de constantes
#define VELOCIDADE_PADRAO 50.0f				// km/h
#define INTERVALO_GERACAO_VEICULO_MS 5000	// Gera um ve�culo a cada 5 segundos
#define MAX_VEICULOS 100
#define QT_ROTAS 3							// Quantidade fixa de rotas

typedef enum {N, S, E, W} e_car_direction;
typedef enum {FRENTE = 0, DIREITA, ESQUERDA} e_car_sentido;

const SDL_Color CAR_COLORS[] = {
	{ 255, 0, 0, 255 },
	{ 255, 255, 0, 255},
	{ 0, 0, 255, 255},
};
const int CAR_COLORS_COUNT = 3;

// Estrutura do Ve�culo
typedef struct {
	int id;
	float x, y;				// Coordenadas do carro
	int width, height;
	short running;			// 0 - parado, 1 - andando
	short orientation;		// orienta��o

	SDL_Color color;		// Cor do carro
	float Velocidade;		// km/h
	int Intersessao;		// Representado por 0,1,2,... correspondentes a interse��es
	int rotas[QT_ROTAS];	// Array de dire��es que o ve�culo ir� fazer
} Vehicle;

int vehicle_count = 0;

Vehicle vehicles[MAX_VEICULOS];


void vVehicleTask(void*);


// Fun��o para gerar rotas aleat�rias
void generate_random_route(int rotas[QT_ROTAS]) {
	for (int i = 0; i < QT_ROTAS; i++) {
		rotas[i] = rand() % 3; // 0, 1 ou 2 representando dire��es diferentes
	}
}
// Fun��o para obter uma interse��o inicial aleat�ria
int get_random_starting_crossing(void) {
	int crossings[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };
	return crossings[rand() % 8];
}


void getRandomInitialPositionOrientation(float* x, float* y, int* i)
{
	static int z = 0;
	*x = rua_inicio[z][0];
	*y = rua_inicio[z][1];
	*i = rua_inicio_o[z];

	if (++z > 7) z = 0;
}

// Tarefa que gera ve�culos periodicamente
void vVehicleGeneratorTask(void* pvParameters) {
	vehicle_count = 0;
	for (;;) {
		// Criar um novo ve�culo
		if (vehicle_count > 7) continue; // limita a 7 veiculos por enquanto

		Vehicle* new_vehicle = pvPortMalloc(sizeof(Vehicle));
		if (new_vehicle != NULL) {
			new_vehicle->id = vehicle_count;

			// Definir a rota aleatoriamente
			generate_random_route(new_vehicle->rotas);

			// Definir velocidade
			new_vehicle->Velocidade = VELOCIDADE_PADRAO;

			// Definir o cruzamento inicial
			new_vehicle->Intersessao = get_random_starting_crossing();

			// Definir ponto inicial
			getRandomInitialPositionOrientation(&new_vehicle->x, &new_vehicle->y, &new_vehicle->orientation);
			//new_vehicle->orientation = i;

			// Definir cor inicial
			int color_idx = rand() % CAR_COLORS_COUNT;
			new_vehicle->color = CAR_COLORS[color_idx];


			// **Imprimir informa��es do ve�culo criado**
			//printf("Veiculo Criado:\n");
			//printf("ID: %d\n", new_vehicle->id);
			//printf("Velocidade: %.2f km/h\n", new_vehicle->Velocidade);
			//printf("Intersecao Inicial: %d\n", new_vehicle->Intersessao);
			//printf("Posi��o inicial: %.2f, %.2f, ori=%d", new_vehicle->x, new_vehicle->y, new_vehicle->orientation);
			//printf("Rotas: [");
			//for (int i = 0; i < QT_ROTAS; i++) {
			//	printf("%d", new_vehicle->rotas[i]);
			//	if (i < QT_ROTAS - 1) {
			//		printf(", ");
			//	}
			//}
			//printf("]\n\n");

			vehicles[vehicle_count] = *new_vehicle;

			// Criar a tarefa do ve�culo
			xTaskCreate(vVehicleTask, "VehicleTask", configMINIMAL_STACK_SIZE, (void*)&vehicles[vehicle_count], 1, NULL);
			vehicle_count++;
		}

		// Aguardar o pr�ximo intervalo de gera��o
		//vTaskDelay(pdMS_TO_TICKS(INTERVALO_GERACAO_VEICULO_MS));
		vTaskDelay(pdMS_TO_TICKS(100));

	}
}


void updatePosition(Vehicle* car, float vel, float sem_ref, float* sem_dist)
{
	float dist = -1.0f;
	if (car->orientation == NORTE)
	{
		car->y -= vel;
		dist = abs(car->y - sem_ref);
	}
	if (car->orientation == SUL)
	{
		car->y += vel;
		dist = abs(car->y - sem_ref);
	}
	if (car->orientation == LESTE)
	{
		car->x += vel;
		dist = abs(car->x - sem_ref);
	}
	if (car->orientation == OESTE)
	{
		car->x -= vel;
		dist = abs(car->x - sem_ref);
	}
	if (sem_dist) *sem_dist = dist;
}

typedef enum { V_Stop = 0, V_Run, V_Sem, V_Crossing, V_ChangeDirection} e_vehicle_state;

void vVehicleTask(void* args)
{
	Vehicle* car = (Vehicle*) args;
	car->running = 1;
	
	int rota_atual = 0;
	int prox_checagem = 0; // dist�ncia at� o sem�foro
	e_vehicle_state estado = V_Run;

	float sem_ref = 0;
	float sem_dist;
	
	// L�gica dos carros
	for (;;)
	{

		// Movimento do carro
		float vel = 1;
		if (car->running)
		{	
			// Verificar se o carro pode avan�ar o sem�foro ou deve parar
			
			// Depois qual dire��o ele vai tomar? Frente, Direita, Esquerda
			

			// Ajustar o proximo ponto, e mudar orienta��o do carro para satisfazer a dire��o (caso, vire esquerda ou direita)

			// Atualiza a posi��o do carro
			if (estado == V_Run)
			{

				//printf("[CAR %d] VRUNNN", car->id);
				updatePosition(car, vel, sem_ref, &sem_dist);
				if (sem_dist <= 3.0f)
					estado = V_Sem; // verifica o estado do sem�foro

			}
			else if (estado == V_Sem)
			{
				// implementar verifica��o dos sem�foros
				estado = V_Crossing;
			}
			else if (estado == V_Crossing)
			{
				// deve ser feito aqui a movimenta��o do carro at� que
				if (car->rotas[rota_atual] == FRENTE);	 // Atualizar a rota
				if (car->rotas[rota_atual] == ESQUERDA); // Atualiza  a rota
				if (car->rotas[rota_atual] == DIREITA);  // Atualiza  a rota
				// muda dire��o do carro se necess�rio

				estado == V_Run;

			}
			
			if (car->x > 800) car->x = 0;
			if (car->y > 800) car->y = 0;
		}
		vTaskDelay(32);
	}
	return;
}


#endif // !CARS_H
