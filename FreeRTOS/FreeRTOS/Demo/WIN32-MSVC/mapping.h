/**
 * @file mapping.h
 * @brief Defini??o de pontos e valores de interesse do cen?rio
 * As constantes definidas referem-se a valores que correspondem partes do cen?rio (asset\bg.jpg)
**/

#pragma once
#ifndef MAPPING_H
#define MAPPING_H

/// Posi??o das faixas de parada
#define NORTE	0
#define SUL		1
#define LESTE	2
#define OESTE	3

#define NONE   -1	// Nada atribuido, indica fim de trajeto
#define SEMA	0
#define SEMB	1
#define SEMC	2
#define SEMD	3

typedef enum { FRENTE = 0, DIREITA, ESQUERDA } e_directions;

// Faixas de parada
#define FP_X1 157
#define FP_X2 247
#define FP_X3 464
#define FP_X4 559
#define FP_Y1 148
#define FP_Y2 246
#define FP_Y3 474
#define FP_Y4 572

const int sema_paradas[4] = {
	FP_Y1, FP_Y2,	// Norte, Sul
	FP_X1, FP_X2	// Leste, Oeste
};

const int semb_paradas[4] = {
	FP_Y1, FP_Y2,	// Norte, Sul
	FP_X3, FP_X4	// Leste, Oeste
};

const int semc_paradas[4] = {
	FP_Y3, FP_Y4,	// Norte, Sul
	FP_X1, FP_X2	// Leste, Oeste
};

const int semd_paradas[4] = {
	FP_Y3, FP_Y4,	// Norte, Sul
	FP_X3, FP_X4	// Leste, Oeste
};


/// Posi??o inicial de gera??o em cada rua (pares x, y)
#define WFAIXA 25 // dimens?es da faixa

// limites de borda das ruas 
#define LIM_X1 50
#define LIM_X2 670
#define LIM_Y1 35
#define LIM_Y2 690

#define R_WE1_Y1 170 // Rua horizontal de cima
#define R_WE1_Y2 225 

#define R_WE2_Y1 500 // Rua horizontal de baixo
#define R_WE2_Y2 550

#define R_NS1_X1 177 // Rua vertical da esquerda
#define R_NS1_X2 230

#define R_NS2_X1 488 // Rua vertical da direita
#define R_NS2_X2 538


const float rua_inicio[8][2] = {
	{LIM_X1, (R_WE1_Y2 - WFAIXA)},	// Entrada Oeste - Cruzamento A
	{R_NS1_X1, LIM_Y1  },			// Entrada Norte - Cruzamento A
	{R_NS2_X1, LIM_Y1  },			// Entrada Norte - Cruzamento B
	{LIM_X2  , R_WE1_Y1},			// Entrada Leste - Cruzamento B
	{LIM_X2  , R_WE2_Y1},			// Entrada Leste - Cruzamento D
	{R_NS2_X2 - WFAIXA, LIM_Y2 },	// Entrada Sul   - Cruzamento D
	{R_NS1_X2 - WFAIXA, LIM_Y2 },	// Entrada Sul   - Cruzamento C
	{LIM_X1  , R_WE2_Y2 - WFAIXA},	// Entrada Oeste - Cruzamento C
};
const int rua_inicio_o[8] = { LESTE, SUL, SUL, OESTE, OESTE, NORTE, NORTE, LESTE };
const int sem_inicio[8] = { SEMA, SEMA, SEMB, SEMB, SEMD, SEMD, SEMC, SEMC };
const char sem2string[][8] = { "A", "B", "C", "D" };
const char ori2string[][10] = { "NORTE", "SUL", "LESTE", "OESTE" };
const char dir2string[][10] = { "FRENTE", "DIREITA", "ESQUERDA" };

/// Pr?ximo Sem?foro
// Mapa de trajeto:  Pr?ximo Sem?foro = SEM_MAP[Sentido do carro][Sem?foro atual][Dire??o]
// Dire??o: Frente = 0, Direita = 1, Esquerda = 2
// Sentido: NORTE = 0, SUL = 1, LESTE = 2, OESTE = 3
const int SEMMAP_NEXT[][4][3] = {
	{	// Caso o sentido do carro seja o NORTE = 0
		{NONE, SEMB, NONE}, // Sem. A
		{NONE, NONE, SEMA}, // Sem. B
		{SEMA, SEMD, NONE},	// Sem. C
		{SEMB, NONE, SEMC}, // Sem. D
	},
	{	// Caso o sentido do carro seja o SUL = 1
		{SEMC, NONE, SEMB}, // Sem. A
		{SEMD, SEMA, NONE}, // Sem. B
		{NONE, NONE, SEMD},	// Sem. C
		{NONE, SEMC, NONE}, // Sem. D
	},
	{	// Caso o sentido do carro seja o LESTE = 2
		{SEMB, SEMC, NONE}, // Sem. A
		{NONE, SEMD, NONE}, // Sem. B
		{SEMD, NONE, SEMB}, // Sem. C
		{NONE, NONE, SEMB}, // Sem. D
	},
	{	// Caso o sentido do carro seja o OESTE = 3
		{NONE, NONE, SEMC}, // Sem. A
		{SEMA, NONE, SEMD}, // Sem. B
		{NONE, SEMA, NONE}, // Sem. C
		{SEMC, SEMB, NONE}, // Sem. 
	}
};

/// Mapa de pontos de paradas
// x ou y = SEMMAP_STOP[sentido][semaforo]
const int SEMMAP_STOP[][4] = {
	{FP_Y2, FP_Y2, FP_Y4, FP_Y4}, // NORTE = 0
	{FP_Y1, FP_Y1, FP_Y3, FP_Y3}, // SUL   = 1
	{FP_X1, FP_X3, FP_X1, FP_X3}, // LESTE = 2
	{FP_X2, FP_X4, FP_X2, FP_X4}, // OESTE = 2
};

/// Apartir de um sentido do carro, qual a proxima orienta??o ap?s uma mudan?a
const int ORIENTATION_CHANGE[][3] = {
	{NORTE, LESTE, OESTE},	// NORTE
	{SUL  , OESTE, LESTE},	// SUL
	{LESTE, SUL  , NORTE},	// LESTE
	{OESTE, NORTE, SUL},	// LESTE
};

// -1 indica fim que n?o h? sem?foro
// 0 indica que h? sem?foro
static const int CROSSING_STEP[4][4][3] = {
	{ // Sem. A
		{-1, R_WE1_Y2 - 10, R_WE1_Y1},		// NORTE - Frente, Direita, Esquerda
		{0 , R_WE1_Y1	  , R_WE1_Y2 - 10},	// SUL   - Frente, Direita, Esquerda
		{0 , R_NS1_X1	  , R_NS1_X2 - 10},	// LESTE - Frente, Direita, Esquerda
		{-1, R_NS1_X2 - 10, R_NS1_X1},	    // SUL	 - Frente, Direita, Esquerda

	},
	{ // Sem. B
		{-1, R_WE1_Y2 - 10, R_WE1_Y1},		// NORTE - Frente, Direita, Esquerda
		{0 , R_WE1_Y1	  , R_WE1_Y2 - 10},	// SUL	 - Frente, Direita, Esquerda
		{-1 , R_NS2_X1	  , R_NS2_X2 - 10},	// LESTE - Frente, Direita, Esquerda
		{0, R_NS2_X2 - 10, R_NS2_X1},	    // SUL	 - Frente, Direita, Esquerda

	},
	{ // Sem. C
		{ 0, R_WE2_Y2 - 10, R_WE2_Y1},		// NORTE - Frente, Direita, Esquerda
		{ 0, R_WE2_Y1	  , R_WE2_Y2 - 10},	// SUL	 - Frente, Direita, Esquerda
		{-1, R_NS1_X1	  , R_NS1_X2 - 10},	// LESTE - Frente, Direita, Esquerda
		{ 0, R_NS1_X2 - 10, R_NS1_X1},	    // SUL	 - Frente, Direita, Esquerda

	},
	{ // Sem. D
		{ 0, R_WE2_Y2 - 10, R_WE2_Y1},		// NORTE - Frente, Direita, Esquerda
		{ 0, R_WE2_Y1	  , R_WE2_Y2 - 10},	// SUL	 - Frente, Direita, Esquerda
		{-1, R_NS2_X1	  , R_NS2_X2 - 10},	// LESTE - Frente, Direita, Esquerda
		{ 0, R_NS2_X2 - 10, R_NS2_X1},	    // SUL	 - Frente, Direita, Esquerda

	},

};

#endif // ! MAPPING_H