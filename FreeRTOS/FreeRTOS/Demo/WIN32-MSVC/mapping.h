/// Pontos de mapeamento do cenário

#pragma once
#ifndef MAPPING_H
#define MAPPING_H

// Faixas de parada
const int FP_X1 = 157;
const int FP_X2 = 247;
const int FP_X3 = 464;
const int FP_X4 = 559;
const int FP_Y1 = 148;
const int FP_Y2 = 246;
const int FP_Y3 = 474;
const int FP_Y4 = 572;

/// Posição das faixas de parada
#define NORTE	0
#define SUL		1
#define LESTE	2
#define OESTE	3

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


/// Posição inicial de geração em cada rua (pares x, y)
#define WFAIXA 25 // dimensões da faixa

// limites de borda das ruas 
const float LIM_X1 = 50;
const float LIM_X2 = 670;
const float LIM_Y1 = 35;
const float LIM_Y2 = 690;
	  
const float R_WE1_Y1 = 170; // Rua horizontal de cima
const float R_WE1_Y2 = 225; 
	  
const float R_WE2_Y1 = 500; // Rua horizontal de baixo
const float R_WE2_Y2 = 550;
	  
const float R_NS1_X1 = 177; // Rua vertical da esquerda
const float R_NS1_X2 = 230;
	  
const float R_NS2_X1 = 448; // Rua vertical da direita
const float R_NS2_X2 = 538;


const float rua_inicio[8][2] = {
	{LIM_X1  , R_WE1_Y2 - WFAIXA},	// Entrada Oeste - Cruzamento A
	{R_NS1_X1, LIM_Y1  },			// Entrada Norte - Cruzamento A
	{R_NS2_X1, LIM_Y1  },			// Entrada Norte - Cruzamento B
	{LIM_X2  , R_WE1_Y1},			// Entrada Leste - Cruzamento B
	{LIM_X2  , R_WE2_Y1},			// Entrada Leste - Cruzamento D
	{R_NS2_X2 - WFAIXA, LIM_Y2 },	// Entrada Sul   - Cruzamento D
	{R_NS1_X2 - WFAIXA, LIM_Y2 },	// Entrada Sul   - Cruzamento C
	{LIM_X1  , R_WE2_Y2 - WFAIXA},	// Entrada Oeste - Cruzamento C
};


#endif // ! MAPPING_H