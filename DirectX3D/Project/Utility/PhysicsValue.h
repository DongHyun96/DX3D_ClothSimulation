#pragma once

//const Vector3 GRAVITY = {0,-980,0};
//const float	   K_DRAG = 0.005f;
//const float	      COR = 1.f; // ALPHA 반발계수 coefficient of restitution
//const float	       MU = 1;
//
//const float K_D					= 0.1f;
//const float DEFAULT_K_SPRING	= 1000.f;

const Vector3 GRAVITY = { 0,-980,0 };
const float	   K_DRAG = 0.05f;	// Viscous Drag
const float	      COR = 0.8f; // 충돌 ALPHA 반발계수 coefficient of restitution
const float	       MU = 1;

const float K_D = 0.1f;
const float DEFAULT_K_SPRING = 1000.f;