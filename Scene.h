#pragma once
#include <iostream>
#include "Node.h"
#include "Geometry.h"
#include "Transform.h"
#include "SkyBox.h"
#include "EnvMappingGeo.h"
#include <learnopengl/shader.h>
#include "MaterialGeo.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "PointLight.h"
#include "TextureGeo.h"
#include "Astro.h"
#include "Lobby.h"
#include <iostream>
#include "DirLight.h"
#include "AstroMotion.h"
#include "Collision.h"
#include <random>
#include <time.h>
#include <algorithm>
#include "particle_generator.h"
#include <irrklang/irrKlang.h>
#include "footstep.h"

using namespace irrklang;
using namespace std;

class Scene
{
	const float PI = 3.14159265358979323846;
	enum class AstroType
	{
		PLAYER, NONPLAYER, BOX
	};
	class COLOR
	{
	public:
		glm::vec3 Black = glm::vec3(0x3E,0x47,0x4E)/255.0f;
		glm::vec3 Blue = glm::vec3(0x13, 0x2E, 0xD1) / 255.0f;
		glm::vec3 Brown = glm::vec3(0x71, 0x49, 0x1D) / 255.0f;
		glm::vec3 Cyan = glm::vec3(0x39, 0xFE, 0xDD) / 255.0f;
		glm::vec3 Dark_Green = glm::vec3(0x13, 0x80, 0x2C) / 255.0f;
		glm::vec3 Lime = glm::vec3(0x4E, 0xEF, 0x38) / 255.0f;
		glm::vec3 Orange = glm::vec3(0xF1, 0x7D, 0x0C) / 255.0f;
		glm::vec3 Pink = glm::vec3(0xEC, 0x54, 0xBB) / 255.0f;
		glm::vec3 Purple = glm::vec3(0x6C, 0x2F, 0xBC) / 255.0f;
		glm::vec3 Red = glm::vec3(0xC5, 0x12, 0x11) / 255.0f;
		glm::vec3 White = glm::vec3(0xD6, 0xDF, 0xF1) / 255.0f;
		glm::vec3 Yellow = glm::vec3(0xF6, 0xF6, 0x57) / 255.0f;
	};
	COLOR color_inst;
	vector<glm::vec3> non_player_color = { color_inst.Black, color_inst.Blue, color_inst.Brown, color_inst.Cyan, color_inst.Dark_Green, color_inst.Lime, color_inst.Pink, color_inst.Purple, color_inst.Red, color_inst.White, color_inst.Yellow };
	mt19937 seed;
public:

	// Shader Program ID
	Shader lighting_shader, lighting_texture_shader;
	Shader lighting_toon_shader, lighting_texture_toon_shader;
	PointLight* point_light;
	DirLight* dir_light;
	Lobby* lobby;
	vector<BoundingPlane> BPs;
	Astro* astro_proto;

	vector<AstroMotion*> players;
	vector<AstroMotion*> nonplayers;
	vector<AstroMotion*> boxes;
	vector<AstroMotion*> disappear_nonplayers;
	vector<float> boxes_x = { -7.32653,-9.7734,10.1497 };
	vector<float> boxes_z = { -0.476075,11.0199,7.82302 };
	//-6.89643 - 12.1222
	//-14.7186 -2.18726
	//-14.7186 17.2242
	//-10.3378 20.1241
	int num_player = 1, num_nonplayer= 9, num_box = 3;
	int new_id;

	Transform* world;
	Transform* astro_height;
	Node* root;
	float t;

	ParticleGenerator* particles;
	Shader particle_shader;
	ISoundEngine* SoundEngine;
	ISound* sound_player;
	FootStep foot_sound;
public:

	Scene() :
		lighting_shader("shaders/lighting.vs", "shaders/lighting.fs"),
		lighting_texture_shader("shaders/lighting_texture.vs", "shaders/lighting_texture.fs"),
		lighting_toon_shader("shaders/lighting_toon.vs", "shaders/lighting_toon.fs"),
		lighting_texture_toon_shader("shaders/lighting_texture_toon.vs", "shaders/lighting_texture_toon.fs"),
		particle_shader("shaders/particle.vs", "shaders/particle.fs"),
		t(0)
	{
		point_light = new PointLight(
			glm::vec3(0, 100, 10),
			glm::vec3(1, 1, 1),
			1.0,
			0.0,
			0.0
		);
		dir_light = new DirLight(
			glm::vec3(0, -1, -1),
			glm::vec3(1)
		);

		lobby = new Lobby("resources/objects/amongus_lobby/amongus_lobby.obj", false);
		//lobby->scale2height(4);
		lobby->use_shader(&lighting_texture_toon_shader);

		astro_proto = new Astro("resources/objects/astronauts/amongus_astro_still.obj", "resources/objects/astronauts/amongus_astro_moving1.obj", "resources/objects/astronauts/amongus_astro_moving2.obj", false);
		astro_proto->use_shader(&lighting_toon_shader);
		
		astro_height = new Transform(glm::translate(glm::mat4(1), glm::vec3(0, 2, 0)));
		new_id = 0;
		for (size_t i = 0; i < num_box; i++)
		{
			add_astro(AstroType::BOX, glm::vec3(), false, boxes_x[i], boxes_z[i], 0);
		}

		for (size_t i = 0; i < num_player; i++)
		{
			add_astro(AstroType::PLAYER, color_inst.Orange, true, 0, 0, 0);
		}
		
		for (size_t i = 0; i < num_nonplayer; i++)
		{
			add_astro(AstroType::NONPLAYER, non_player_color[new_id], true, 3, 0, 0);
		}
		for (size_t i = 1; i < num_nonplayer; i++)
		{
			nonplayers[i]->disappear();
		}
		
		vector<BoundingPlane> measured_BP = {
			BoundingPlane(glm::vec3(0,0,-13), glm::vec3(0,0,1)),
			BoundingPlane(glm::vec3(-7,0,-13), glm::vec3(10.8,0,8)),
			BoundingPlane(glm::vec3(-15,0,0), glm::vec3(1,0,0)),
			BoundingPlane(glm::vec3(-15,0,17), glm::vec3(3.5,0,-4.7)),
			BoundingPlane(glm::vec3(0,0,20.5), glm::vec3(0,0,-1)),
			BoundingPlane(glm::vec3(15,0,17), glm::vec3(-3.5,0,-4.7)),
			BoundingPlane(glm::vec3(15,0,0), glm::vec3(-1,0,0)),
			BoundingPlane(glm::vec3(7,0,-13), glm::vec3(-10.8,0,8)),
			BoundingPlane(glm::vec3(0,0,4.67), glm::vec3(0,0,-1))
		};
		BPs = measured_BP;
		
		build_scene_graph();

		root = world;

		init_particle();
		for (size_t i = 0; i < nonplayers.size(); i++)
		{
			cout << nonplayers[i]->child.size();
		}
		gen_seed();
		SoundEngine = createIrrKlangDevice();

	}

	~Scene() { delete root; delete particles; delete SoundEngine; };

	void init_particle()
	{
		auto particle_tex = loadTextureFromFile("resources/textures/particle.png", true);
		for (size_t i = 0; i < nonplayers.size(); i++)
		{
			nonplayers[i]->addParticles(&particle_shader, particle_tex, 200);
		}
	}

	void gen_seed()
	{
		std::random_device rd;  //Will be used to obtain a seed for the random number engine
		std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
		seed = gen;
		srand(time(NULL));
	}

	void build_scene_graph()
	{
		/***********then build the scene graph***************/
		world = new Transform(glm::mat4(1.0f));
		world->addChild(lobby);
		world->addChild(astro_height);

		vector<AstroMotion*> all_astro;
		all_astro.reserve(boxes.size() + players.size() + nonplayers.size());
		all_astro.insert(all_astro.end(), boxes.begin(), boxes.end());
		all_astro.insert(all_astro.end(), players.begin(), players.end());
		all_astro.insert(all_astro.end(), nonplayers.begin(), nonplayers.end());
		for (size_t i = 0; i < all_astro.size(); i++)
		{
			astro_height->addChild(all_astro[i]);
		}
		
	}

	void draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos)
	{
		lighting_shader.use();
		lighting_shader.setMat4("view", view);
		lighting_shader.setMat4("projection", projection);
		lighting_shader.setVec3("viewPos", cameraPos);
		lighting_shader.setVec3("pointlight.position", point_light->position);
		lighting_shader.setVec3("pointlight.color", point_light->color);
		lighting_shader.setFloat("pointlight.constant", point_light->constant);
		lighting_shader.setFloat("pointlight.linear", point_light->linear);
		lighting_shader.setFloat("pointlight.quadratic", point_light->quadratic);

		lighting_texture_shader.use();
		lighting_texture_shader.setMat4("view", view);
		lighting_texture_shader.setMat4("projection", projection);
		lighting_texture_shader.setVec3("viewPos", cameraPos);
		lighting_texture_shader.setVec3("pointlight.position", point_light->position);
		lighting_texture_shader.setVec3("pointlight.color", point_light->color);
		lighting_texture_shader.setFloat("pointlight.constant", point_light->constant);
		lighting_texture_shader.setFloat("pointlight.linear", point_light->linear);
		lighting_texture_shader.setFloat("pointlight.quadratic", point_light->quadratic);

		lighting_toon_shader.use();
		lighting_toon_shader.setMat4("view", view);
		lighting_toon_shader.setMat4("projection", projection);
		lighting_toon_shader.setVec3("viewPos", cameraPos);
		lighting_toon_shader.setVec3("dirlight.direction", dir_light->direction);
		lighting_toon_shader.setVec3("dirlight.color", dir_light->color);

		lighting_texture_toon_shader.use();
		lighting_texture_toon_shader.setMat4("view", view);
		lighting_texture_toon_shader.setMat4("projection", projection);
		lighting_texture_toon_shader.setVec3("viewPos", cameraPos);
		lighting_texture_toon_shader.setVec3("dirlight.direction", dir_light->direction);
		lighting_texture_toon_shader.setVec3("dirlight.color", dir_light->color);

		root->draw(glm::mat4(1.0f));

		particle_shader.use();
		particle_shader.setMat4("view", view);
		particle_shader.setMat4("projection", projection);
		for (size_t i = 0; i < nonplayers.size(); i++)
		{
			nonplayers[i]->particles->Draw();
		}
	}

	void update(float dt) // update all nodes every time step
	{
		t += dt;	

		for (size_t i = 0; i < nonplayers.size(); i++)
		{
			if (t-nonplayers[i]->status_check_time >5)
			{
				if (rand() % 3 == 0)
				{
					if (nonplayers[i]->disappear_flag) 
					{// change disappear to appear
						nonplayers[i]->appear();
						glm::vec3 normal;
						if (collision_check(nonplayers[i], normal))
						{
							uniform_real_distribution<> dis_x(-7.8, 7.8);
							uniform_real_distribution<> dis_z(-7.0, 16.0);
							uniform_real_distribution<> dis_yaw(-PI, PI);
							do
							{
								nonplayers[i]->x = dis_x(seed);
								nonplayers[i]->z = dis_z(seed);
								nonplayers[i]->yaw = dis_yaw(seed);
							} while (collision_check(nonplayers[i], normal));
						}
						SoundEngine->play2D("resources/audio/bleep.mp3", false);
					}
					else
					{
						nonplayers[i]->disappear();
						SoundEngine->play2D("resources/audio/powerup.wav", false);
					}
					
				}
				nonplayers[i]->status_check_time = t;
			}
			if (t - nonplayers[i]->run_check_time > 2)
			{
				if (rand() % 3 == 0)
				{
					if (nonplayers[i]->run_flag)
					{// change disappear to appear
						nonplayers[i]->stop();
					}
					else
					{
						nonplayers[i]->start();
					}

				}
				nonplayers[i]->run_check_time = t;
			}
		}

		vector<AstroMotion*> all_astro;
		all_astro.reserve(players.size() + nonplayers.size());
		all_astro.insert(all_astro.end(), players.begin(), players.end());
		all_astro.insert(all_astro.end(), nonplayers.begin(), nonplayers.end());

		for (size_t i = 0; i < all_astro.size(); i++)
		{

			all_astro[i]->step(dt);
			glm::vec3 normal;
			if (collision_check(all_astro[i], normal))
			{
				all_astro[i]->react_collision(dt, normal);
			}

			all_astro[i]->poststep(dt);

		}
		if (players[0]->run_flag) foot_sound.update(dt);
	}

	float trangle_func(float t, float T, float F, float offset_t, float offset_f)
	{
		t = t + offset_t;
		t = t - floor(t/T) * T;
		float deriv = 2 * F / T;
		float f = 0;
		if (t < T/2)
		{
			f = deriv * t;
		}
		else {
			f = 2 * F - deriv * t;
		}
		f = f + offset_f;
		return f;
	}

	glm::mat4 get_transform2world(Transform* node)
	{
		auto T = glm::mat4(1);
		while (node != nullptr)
		{
			T = node->model * T;
			node = (Transform*)node->parent;
		}
		return T;
	}

	void add_astro(AstroType astro_type, glm::vec3 color, bool random_position = true, float x=0, float z=0, float yaw = 0) 
	{
		auto astro_geo = new Astro(*astro_proto);
		astro_geo->color = color;
		auto astro_trans = new AstroMotion(x, z, yaw, 0, astro_geo);
		
		// random generate se2 configuration
		glm::vec3 normal;
		if (random_position)
		{
			uniform_real_distribution<> dis_x(-7.8, 7.8);
			uniform_real_distribution<> dis_z(-7.0, 16.0);
			uniform_real_distribution<> dis_yaw(-PI, PI);
			do
			{
				astro_trans->x = dis_x(seed);
				astro_trans->z = dis_z(seed);
				astro_trans->yaw = dis_yaw(seed);
			} while (collision_check(astro_trans, normal));
		}

		
		if (astro_type == AstroType::BOX)
		{
			astro_trans->player = false;
			astro_geo->visible = false;
			astro_geo->update(glm::scale(glm::mat4(1), glm::vec3(2.5)));
			boxes.push_back(astro_trans);

		}
		else if (astro_type == AstroType::PLAYER)
		{
			astro_trans->player = true;
			astro_geo->visible = true;
			astro_trans->stop();
			players.push_back(astro_trans);
		}
		else if (astro_type == AstroType::NONPLAYER)
		{
			astro_trans->player = false;
			astro_geo->visible = true;
			astro_trans->start();
			nonplayers.push_back(astro_trans);
		}
		new_id = (new_id+1)% non_player_color.size();
	}

	bool collision_check(AstroMotion* astro_a, glm::vec3& normal)
	{
		BoundingSphere BS1 = astro_a->get_BS();
		BoundingSphere BS2;

		vector<AstroMotion*> all_astro;
		all_astro.reserve(boxes.size() + players.size() + nonplayers.size());
		all_astro.insert(all_astro.end(), boxes.begin(), boxes.end());
		all_astro.insert(all_astro.end(), players.begin(), players.end());
		all_astro.insert(all_astro.end(), nonplayers.begin(), nonplayers.end());

		// check collision with other bounding spheres
		for (size_t i = 0; i < all_astro.size(); i++)
		{
			if (all_astro[i] == astro_a or all_astro[i]->disappear_flag)
			{
				continue;
			}
			BS2 = all_astro[i]->get_BS();

			if (BS_inter_BS(BS1, BS2))
			{
				normal = BS1.center - BS2.center;
				return true;
			}
		}

		// check collision with bounding box
		for (size_t i = 0; i < BPs.size(); i++)
		{
			if (BS_inter_BP(BS1, BPs[i]))
			{
				cout << "collision with bounding box " << i << endl;
				normal = BPs[i].n;
				return true;
			}
		}
		return false;
	}
};