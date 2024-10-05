#pragma once

#include "task.h"
#include "SDL_pixels.h"
#include "mapping.h"

#include <time.h>

#ifndef CARS_H
#define CARS_H

// -------------------------------------------------------------------------------

#define NUM_CROSSINGS 4
#define QT_ROTAS 4
#define PHASE_DURATION_MS 10000 // 30 segundos

typedef struct {
	char id; // 'A', 'B', 'C', 'D'
	SemaphoreHandle_t sem_NS_Straight;
	SemaphoreHandle_t sem_EW_Straight;
	SemaphoreHandle_t sem_NS_Left;
	SemaphoreHandle_t sem_EW_Left;
	int currentPhase;      // Fase atual do ciclo semaf?rico
	int initialPhase;      // Fase inicial ao iniciar o ciclo
} Intersection;

Intersection intersections[NUM_CROSSINGS];

// Fun??o de Inicializa??o dos Cruzamentos

void init_intersections() {

	char ids[NUM_CROSSINGS] = { 'A', 'B', 'C', 'D' };
	// Fases iniciais para cada cruzamento
	int initialPhases[NUM_CROSSINGS] = { 0, 1, 2, 3 };

	for (int i = 0; i < NUM_CROSSINGS; i++) {
		intersections[i].id = ids[i];
		intersections[i].sem_NS_Straight = xSemaphoreCreateBinary();
		intersections[i].sem_EW_Straight = xSemaphoreCreateBinary();
		intersections[i].sem_NS_Left = xSemaphoreCreateBinary();
		intersections[i].sem_EW_Left = xSemaphoreCreateBinary();
		intersections[i].initialPhase = initialPhases[i];
		intersections[i].currentPhase = intersections[i].initialPhase; // Define a fase atual como a fase inicial
	}
}

// Fun??o para Gerenciar as Fases dos Sem?foros

void manage_semaphores(Intersection* crossing) {
	while (1) {
		// Dependendo da fase atual, ativa o sem?foro correspondente
		switch (crossing->currentPhase) {
		case 0:
			// Fase 1: NS Straight disponivel
			xSemaphoreGive(crossing->sem_NS_Straight);
			xSemaphoreTake(crossing->sem_EW_Left, 0);
			xSemaphoreTake(crossing->sem_EW_Straight, 0);
			xSemaphoreTake(crossing->sem_NS_Left, 0);
			printf("Cruzamento %c: Pode Frente e Direita  N->S e S->N \n", crossing->id);
			break;
		case 1:
			// Fase 2: EW Left dispon?vel
			xSemaphoreGive(crossing->sem_EW_Left);
			xSemaphoreTake(crossing->sem_NS_Straight, 0);
			xSemaphoreTake(crossing->sem_EW_Straight, 0);
			xSemaphoreTake(crossing->sem_NS_Left, 0);
			printf("Cruzamento %c: Pode Esquerda E->S e W->N \n", crossing->id);
			break;
		case 2:
			// Fase 3: EW Straight dispon?vel
			xSemaphoreGive(crossing->sem_EW_Straight);
			xSemaphoreTake(crossing->sem_NS_Straight, 0);
			xSemaphoreTake(crossing->sem_EW_Left, 0);
			xSemaphoreTake(crossing->sem_NS_Left, 0);
			printf("Cruzamento %c: Pode Frente e Direita E->W e W->E \n", crossing->id);
			break;
		case 3:
			// Fase 4: NS Left dispon?vel
			xSemaphoreGive(crossing->sem_NS_Left);
			xSemaphoreTake(crossing->sem_NS_Straight, 0);
			xSemaphoreTake(crossing->sem_EW_Left, 0);
			xSemaphoreTake(crossing->sem_EW_Straight, 0);
			printf("Cruzamento %c: Pode Esquerda N->E e S->W \n", crossing->id);
			break;
		default:
			// Reseta para a fase 0 caso ocorra algum erro
			crossing->currentPhase = 0;
			continue;
		}

		// Aguarda a dura??o da fase
		vTaskDelay(pdMS_TO_TICKS(PHASE_DURATION_MS));

		// Passa para a pr?xima fase
		crossing->currentPhase = (crossing->currentPhase + 1) % 4;
	}
}

// Fun??es das Tarefas para Cada Cruzamento

void vCruzamentoATask(void* pvParameters) {
	Intersection* crossing = &intersections[0];
	manage_semaphores(crossing);
}

void vCruzamentoBTask(void* pvParameters) {
	Intersection* crossing = &intersections[1];
	manage_semaphores(crossing);
}

void vCruzamentoCTask(void* pvParameters) {
	Intersection* crossing = &intersections[2];
	manage_semaphores(crossing);
}

void vCruzamentoDTask(void* pvParameters) {
	Intersection* crossing = &intersections[3];
	manage_semaphores(crossing);
}

// ------------------------------------------------------------------------------


// Definições de constantes
#define VELOCIDADE_PADRAO 50.0f				// km/h
#define INTERVALO_GERACAO_VEICULO_MS 5000	// Gera um veículo a cada 5 segundos
#define MAX_VEICULOS 100
#define QT_ROTAS 3							// Quantidade fixa de rotas

typedef enum { N, S, E, W } e_car_direction;


const SDL_Color CAR_COLORS[] = {
	{ 255, 0, 0, 255 },
	{ 255, 255, 0, 255},
	{ 0, 0, 255, 255},
};
const int CAR_COLORS_COUNT = 3;

// Estrutura do Veículo
typedef struct {
	int id;
	float x, y;				// Coordenadas do carro
	int width, height;
	short running;			// 0 - parado, 1 - andando
	short orientation;		// orientação

	SDL_Color color;		// Cor do carro
	float Velocidade;		// km/h
	int Intersessao;		// Representado por 0,1,2,... correspondentes a interseções
	int rotas[QT_ROTAS];	// Array de direções que o veículo irá fazer
} Vehicle;

int vehicle_count = 0;

Vehicle vehicles[MAX_VEICULOS];


void vVehicleTask(void*);


// Função para gerar rotas aleatórias
void generate_random_route(int rotas[QT_ROTAS]) {
	for (int i = 0; i < QT_ROTAS; i++) {
		rotas[i] = rand() % 3; // 0, 1 ou 2 representando direções diferentes
	}
}
// Função para obter uma interseção inicial aleatória
int get_random_starting_crossing(void) {
	static int i = 0;
	if (i > 7) i = 0;
	return i++; // interrompe a aleatoriedade para debug

	return rand() % 8;
}


void getRandomInitialPositionOrientation(int z, float* x, float* y, int* i)
{
	//static int z = 0;
	*x = rua_inicio[z][0];
	*y = rua_inicio[z][1];
	*i = rua_inicio_o[z];

	//if (++z > 7) z = 0;
}

// Tarefa que gera veículos periodicamente
void vVehicleGeneratorTask(void* pvParameters) {
	vehicle_count = 0;
	for (;;) {
		// Criar um novo veículo
		if (vehicle_count > 20) { // limita a 7 veiculos por enquanto
			vTaskDelay(5000);
			continue;
		}

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
			getRandomInitialPositionOrientation(new_vehicle->Intersessao, &new_vehicle->x, &new_vehicle->y, &new_vehicle->orientation);
			//new_vehicle->orientation = i;

			// Definir cor inicial
			int color_idx = rand() % CAR_COLORS_COUNT;
			new_vehicle->color = CAR_COLORS[color_idx];


			// **Imprimir informações do veículo criado**
			//printf("Veiculo Criado:\n");
			//printf("ID: %d\n", new_vehicle->id);
			//printf("Velocidade: %.2f km/h\n", new_vehicle->Velocidade);
			//printf("Intersecao Inicial: %d\n", new_vehicle->Intersessao);
			//printf("Posição inicial: %.2f, %.2f, ori=%d", new_vehicle->x, new_vehicle->y, new_vehicle->orientation);
			//printf("Rotas: [");
			//for (int i = 0; i < QT_ROTAS; i++) {
			//	printf("%d", new_vehicle->rotas[i]);
			//	if (i < QT_ROTAS - 1) {
			//		printf(", ");
			//	}
			//}
			//printf("]\n\n");

			vehicles[vehicle_count] = *new_vehicle;

			// Criar a tarefa do veículo
			xTaskCreate(vVehicleTask, "VehicleTask", configMINIMAL_STACK_SIZE, (void*)&vehicles[vehicle_count], 1, NULL);
			vehicle_count++;
		}

		// Aguardar o próximo intervalo de geração
		//vTaskDelay(pdMS_TO_TICKS(INTERVALO_GERACAO_VEICULO_MS));
		vTaskDelay(pdMS_TO_TICKS(100));

	}
}

/**
 * @brief Atualiza a posição do veículo e computa a distância até o objetivo.
 *
 * Se out_dist for nulo, apenas atualiza a posição do veículo
 *
 * @param[in,out]	car			Referência do veículo
 * @param[in]		vel			Velocidade do veículo
 * @param[in]		goal		Distância alvo ao longo do trajeto
 * @param[out]		out_dist	Distância atual do veículo até o objetivo
**/
void updatePosition(Vehicle* car, float vel, float goal, float* out_dist)
{
	float dist = -1.0f;
	if (car->orientation == NORTE)
	{
		car->y -= vel;
		dist = abs(car->y - goal);
	}
	if (car->orientation == SUL)
	{
		car->y += vel;
		dist = abs(car->y - goal);
	}
	if (car->orientation == LESTE)
	{
		car->x += vel;
		dist = abs(car->x - goal);
	}
	if (car->orientation == OESTE)
	{
		car->x -= vel;
		dist = abs(car->x - goal);
	}
	if (out_dist) *out_dist = dist;
}

typedef enum { V_Stop = 0, V_Run, V_Sem, V_Crossing, V_Leaving } e_vehicle_state;

// @brief Verifica se o veículo deixou a região do cenário
int checkOutBorder(Vehicle* v)
{
	if (v->orientation == NORTE || v->orientation == SUL)
	{
		if (v->y < LIM_Y1 || v->y > LIM_Y2) return 1;
	}
	else if (v->x < LIM_X1 || v->x > LIM_X2) return 1;
	return 0;
}


/**
 * @brief Task do veículo.
 *
 * Simula o comportamento do veículo no cenário. O veículo interage com os semáforos para
 * saber se pode avançar ou se deve esperar.
 *
 * @param[in,out]	args	Referência do veículo instânciado em um container
**/
void vVehicleTask(void* args)
{
	Vehicle* car = (Vehicle*)args;
	car->running = 1;

	int rota_atual = 0;
	int prox_checagem = 0; // distância até o semáforo

	int sem_atual = sem_inicio[car->Intersessao];
	//printf("[CAR % d] sem % s\n", car->id, sem2string[sem_atual]);

	float sem_dist = -1.0f;
	float sem_ref = (float)SEMMAP_STOP[car->orientation][sem_atual];

	short isStateChanged = 0;

	e_vehicle_state estado = V_Run;
	// Lógica dos carros
	for (;;)
	{
		// Movimento do carro
		float vel = 1;
		if (car->running)
		{
			// Verificar se o carro pode avançar o semáforo ou deve parar

			// Depois qual direção ele vai tomar? Frente, Direita, Esquerda


			// Ajustar o proximo ponto, e mudar orientação do carro para satisfazer a direção (caso, vire esquerda ou direita)

			// Atualiza a posição do carro
			if (estado == V_Run)
			{
				//printf("[CAR %d] VRUNNN", car->id);
				updatePosition(car, vel, sem_ref, &sem_dist);
				if (sem_dist <= 3.0f)
				{
					estado = V_Sem; // verifica o estado do semáforo
					isStateChanged = 1;
				}
			}
			else if (estado == V_Sem)
			{
				// implementar verificação dos semáforos


				//Analisa Semáforos de Oeste Para Leste nos 4 cruzamentos

				if ((sem_atual == SEMA) && (car->orientation == LESTE)) {
					if ((car->rotas[rota_atual] == FRENTE) || (car->rotas[rota_atual] == DIREITA)) {
						xSemaphoreTake(intersections[0].sem_EW_Straight, portMAX_DELAY);
					}
					if (car->rotas[rota_atual] == ESQUERDA) {
						xSemaphoreTake(intersections[0].sem_EW_Left, portMAX_DELAY);
					}
				}

				if ((sem_atual == SEMB) && (car->orientation == LESTE)) {
					if ((car->rotas[rota_atual] == FRENTE) || (car->rotas[rota_atual] == DIREITA)) {
						xSemaphoreTake(intersections[1].sem_EW_Straight, portMAX_DELAY);
					}
					if (car->rotas[rota_atual] == ESQUERDA) {
						xSemaphoreTake(intersections[1].sem_EW_Left, portMAX_DELAY);
					}
				}

				if ((sem_atual == SEMC) && (car->orientation == LESTE)) {
					if ((car->rotas[rota_atual] == FRENTE) || (car->rotas[rota_atual] == DIREITA)) {
						xSemaphoreTake(intersections[2].sem_EW_Straight, portMAX_DELAY);
					}
					if (car->rotas[rota_atual] == ESQUERDA) {
						xSemaphoreTake(intersections[2].sem_EW_Left, portMAX_DELAY);
					}
				}

				if ((sem_atual == SEMD) && (car->orientation == LESTE)) {
					if ((car->rotas[rota_atual] == FRENTE) || (car->rotas[rota_atual] == DIREITA)) {
						xSemaphoreTake(intersections[3].sem_EW_Straight, portMAX_DELAY);
					}
					if (car->rotas[rota_atual] == ESQUERDA) {
						xSemaphoreTake(intersections[3].sem_EW_Left, portMAX_DELAY);
					}
				}

				//Analisa Semáforos de Norte Para Sul nos 4 cruzamentos

				if ((sem_atual == SEMA) && (car->orientation == SUL)) {
					if ((car->rotas[rota_atual] == FRENTE) || (car->rotas[rota_atual] == DIREITA)) {
						xSemaphoreTake(intersections[0].sem_NS_Straight, portMAX_DELAY);
					}
					if (car->rotas[rota_atual] == ESQUERDA) {
						xSemaphoreTake(intersections[0].sem_NS_Left, portMAX_DELAY);
					}
				}

				if ((sem_atual == SEMB) && (car->orientation == SUL)) {
					if ((car->rotas[rota_atual] == FRENTE) || (car->rotas[rota_atual] == DIREITA)) {
						xSemaphoreTake(intersections[1].sem_NS_Straight, portMAX_DELAY);
					}
					if (car->rotas[rota_atual] == ESQUERDA) {
						xSemaphoreTake(intersections[1].sem_NS_Left, portMAX_DELAY);
					}
				}

				if ((sem_atual == SEMC) && (car->orientation == SUL)) {
					if ((car->rotas[rota_atual] == FRENTE) || (car->rotas[rota_atual] == DIREITA)) {
						xSemaphoreTake(intersections[2].sem_NS_Straight, portMAX_DELAY);
					}
					if (car->rotas[rota_atual] == ESQUERDA) {
						xSemaphoreTake(intersections[2].sem_NS_Left, portMAX_DELAY);
					}
				}

				if ((sem_atual == SEMD) && (car->orientation == SUL)) {
					if ((car->rotas[rota_atual] == FRENTE) || (car->rotas[rota_atual] == DIREITA)) {
						xSemaphoreTake(intersections[3].sem_NS_Straight, portMAX_DELAY);
					}
					if (car->rotas[rota_atual] == ESQUERDA) {
						xSemaphoreTake(intersections[3].sem_NS_Left, portMAX_DELAY);
					}
				}

				//Analisa Semáforos de Leste Para Oeste nos 4 cruzamentos

				if ((sem_atual == SEMA) && (car->orientation == OESTE)) {
					if ((car->rotas[rota_atual] == FRENTE) || (car->rotas[rota_atual] == DIREITA)) {
						xSemaphoreTake(intersections[0].sem_EW_Straight, portMAX_DELAY);
					}
					if (car->rotas[rota_atual] == ESQUERDA) {
						xSemaphoreTake(intersections[0].sem_EW_Left, portMAX_DELAY);
					}
				}

				if ((sem_atual == SEMB) && (car->orientation == OESTE)) {
					if ((car->rotas[rota_atual] == FRENTE) || (car->rotas[rota_atual] == DIREITA)) {
						xSemaphoreTake(intersections[1].sem_EW_Straight, portMAX_DELAY);
					}
					if (car->rotas[rota_atual] == ESQUERDA) {
						xSemaphoreTake(intersections[1].sem_EW_Left, portMAX_DELAY);
					}
				}

				if ((sem_atual == SEMC) && (car->orientation == OESTE)) {
					if ((car->rotas[rota_atual] == FRENTE) || (car->rotas[rota_atual] == DIREITA)) {
						xSemaphoreTake(intersections[2].sem_EW_Straight, portMAX_DELAY);
					}
					if (car->rotas[rota_atual] == ESQUERDA) {
						xSemaphoreTake(intersections[2].sem_EW_Left, portMAX_DELAY);
					}
				}

				if ((sem_atual == SEMD) && (car->orientation == OESTE)) {
					if ((car->rotas[rota_atual] == FRENTE) || (car->rotas[rota_atual] == DIREITA)) {
						xSemaphoreTake(intersections[3].sem_EW_Straight, portMAX_DELAY);
					}
					if (car->rotas[rota_atual] == ESQUERDA) {
						xSemaphoreTake(intersections[3].sem_EW_Left, portMAX_DELAY);
					}
				}

				//Analisa Semáforos de SUL Para Norte nos 4 cruzamentos

				if ((sem_atual == SEMA) && (car->orientation == NORTE)) {
					if ((car->rotas[rota_atual] == FRENTE) || (car->rotas[rota_atual] == DIREITA)) {
						xSemaphoreTake(intersections[0].sem_NS_Straight, portMAX_DELAY);
					}
					if (car->rotas[rota_atual] == ESQUERDA) {
						xSemaphoreTake(intersections[0].sem_NS_Left, portMAX_DELAY);
					}
				}

				if ((sem_atual == SEMB) && (car->orientation == NORTE)) {
					if ((car->rotas[rota_atual] == FRENTE) || (car->rotas[rota_atual] == DIREITA)) {
						xSemaphoreTake(intersections[1].sem_NS_Straight, portMAX_DELAY);
					}
					if (car->rotas[rota_atual] == ESQUERDA) {
						xSemaphoreTake(intersections[1].sem_NS_Left, portMAX_DELAY);
					}
				}

				if ((sem_atual == SEMC) && (car->orientation == NORTE)) {
					if ((car->rotas[rota_atual] == FRENTE) || (car->rotas[rota_atual] == DIREITA)) {
						xSemaphoreTake(intersections[2].sem_NS_Straight, portMAX_DELAY);
					}
					if (car->rotas[rota_atual] == ESQUERDA) {
						xSemaphoreTake(intersections[2].sem_NS_Left, portMAX_DELAY);
					}
				}

				if ((sem_atual == SEMD) && (car->orientation == NORTE)) {
					if ((car->rotas[rota_atual] == FRENTE) || (car->rotas[rota_atual] == DIREITA)) {
						xSemaphoreTake(intersections[3].sem_NS_Straight, portMAX_DELAY);
					}
					if (car->rotas[rota_atual] == ESQUERDA) {
						xSemaphoreTake(intersections[3].sem_NS_Left, portMAX_DELAY);
					}
				}
				
					sem_ref = 0;

				// Após o semáforo abrir, atualiza a distância alvo (Simular conversão no cruzamento)
				{
					const int temp = CROSSING_STEP[sem_atual][car->orientation][car->rotas[rota_atual]];
					sem_ref = (float)temp;
					estado = V_Crossing;
					isStateChanged = 1;
				}
			}
			else if (estado == V_Crossing)
			{
				// Realiza a conversão no cruzamento (Movimentação do carro no cruzamento)

				// deve ser feito aqui a movimentação do carro até que
				float goal_dist = -1.0f;
				updatePosition(car, vel, sem_ref, &goal_dist);

				//if (car->id == 1) printf("[CAR 1] goal dist = %.2f (%.2f) | sem %s | dir: %d | rota = %d\n", sem_dist, sem_ref, sem2string[sem_atual], car->orientation, car->rotas[rota_atual]);

				// muda de estado
				if (goal_dist <= 1.5f) {

					// Atualiza qual o prox. semáforo na rota
					sem_atual = SEMMAP_NEXT[car->orientation][sem_atual][car->rotas[rota_atual]];

					// Atualiza a orientação do veículo (se fez curva)
					car->orientation = (short)ORIENTATION_CHANGE[car->orientation][car->rotas[rota_atual]];

					// e qual o prox. ponto alvo (p/ verificação de estado)
					sem_ref = SEMMAP_STOP[car->orientation][sem_atual];

					if (sem_atual == NONE) {
						//printf("[CAR %d] Veículo deixando cruzamento\n", car->id);
						estado = V_Leaving;
					}
					else {
						//printf("[CAR %d] Indo para o sem. %s\n", car->id, sem2string[sem_atual]);
						estado = V_Run;
					}
					rota_atual++;
					isStateChanged = 1;
				}
			}
			else if (V_Leaving)
			{

				updatePosition(car, vel, 0, NULL);
				if (checkOutBorder(car))
				{
					car->running = 0;
				}
			}

			if (isStateChanged)
			{
				//printf("[CAR %d] Mudou para o estado %d\n", car->id, estado);
				isStateChanged = 0;
				//if (estado == V_Crossing)
					//printf("[CAR %d] Realizando conversão à %s no cruzamento %c\n", car->id, dir2string[car->rotas[rota_atual]], sem2string[sem_atual]);
				//if (estado == V_Leaving)
					//printf("[CAR %d] Veículo deixando simulação\n", car->id);
			}

			if (car->x > 800) car->x = 0;
			if (car->y > 800) car->y = 0;
		}
		vTaskDelay(32);
	}
	return;
}

#endif // !CARS_H