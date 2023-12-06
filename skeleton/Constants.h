#pragma once
#define M_PI 3.14159265358979323846
#define AIR_DENSITY 1.225
#define EXPANSION_VELOCITY 343
#define MAX_PARTICLES INT_MAX
#define NUM_PARTICLES_SPHERE 500
#define RADIUS_SPHERE 50.0f
#define MAX_SIZE 5.0f

enum GAME_MODES { NORMAL, PERSONALIZATION, END, END2, END3, GAME_MODES_SIZE };
#define GAME_TIME 10.0f
#define END_TIME 5.0f
#define END_TIME2 END_TIME * 2

#define RANDOM_GENERATOR_ORIGIN Vector3(0.0f, 200.0f, 0.0f)
#define FIREWORK_GENERATOR_ORIGIN Vector3(0.0f, 500.0f, 200.0f)

#define SPACECRAFT_MOVEMENT_SPEED 2.0f
#define PROPELLANT1_POSITION Vector3(-2.0f, -6.0f, 0.0f)
#define PROPELLANT2_POSITION Vector3(2.0f, -6.0f, 0.0f)
#define PROPELLANT1_GENERATOR_POSITION Vector3(-2.0f, -8.0f, 0.0f)
#define PROPELLANT2_GENERATOR_POSITION Vector3(2.0f, -8.0f, 0.0f)
#define PROPULSION_FORCE Vector3(0.0f, 50.0f, 0.0f)
#define WINDOW_POSITION Vector3(0.0f, 0.0f, -3.0f)
#define WATER_PLANE_POSITION Vector3(0.0f, 2000.0, 0.0f)

#define CAMERA_INITIAL_AZIMUTH 8.0f		// Ángulo horizontal (azimut)
#define CAMERA_INITIAL_ELEVATION -10.0f	// Ángulo vertical (elevación)
#define CAMERA_INITIAL_RADIUS 30.0f		// Distancia de la cámara al punto de interés
#define CAMERA_POSITION PxVec3(0.0f, 15.0f, -50.0f)
#define CAMERA_VIEW PxVec3(0.0f, 0.0f, 1.0f)
#define CAMERA_FINAL_POSITION Vector3(0.0f, 1000.0f, 200.0f)
#define CAMERA_VIEW2 PxVec3(1.0f, 0.0f, 0.0f)
#define CAMERA_FINAL_POSITION2 Vector3(0.0f, 2000.0f, 0.0f)