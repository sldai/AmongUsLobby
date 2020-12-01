#pragma once
#include "Transform.h"
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Collision.h"
#include "Astro.h"
#include "particle_generator.h"

glm::mat4 se2_mat(float x, float z, float yaw)
{
	return glm::translate(glm::mat4(1), glm::vec3(x, 0, z)) * glm::rotate(glm::mat4(1), yaw, glm::vec3(0,1,0));
}

class AstroMotion : public Transform
{
public:
	float x, z, yaw, v;
	float default_v = 1;
	Astro* astro_ptr; // each AstroMotion should point to a real Astro model
	bool player; // whether user control
	bool disappear_flag = false;
	float status_check_time;
	bool run_flag = true;
	float run_check_time;
	float disappear_ani_time;
	float appear_ani_time;
	ParticleGenerator* particles;
	float run_time, freq=2.0;
	float ani_switch_time = 0.0;
public:
	AstroMotion(float x, float z, float yaw, float v, Astro* a): Transform(se2_mat(x,z,yaw)), x(x), z(z), yaw(yaw), v(v), astro_ptr(a), player(false), appear_ani_time(0), disappear_ani_time(0)
	{
		this->addChild(a);
	}
	
	~AstroMotion() {
		delete astro_ptr;
		astro_ptr = nullptr;
		delete particles;
		particles = nullptr;
	}

	void addParticles(Shader* shader, Texture2D texture, unsigned int amount)
	{
		particles = new ParticleGenerator(shader, texture, amount);
	}

	void step(float dt)
	{
		float dx = dt * v * sin(yaw);
		float dz = dt * v * cos(yaw);
		x += dx;
		z += dz;
		model = se2_mat(x,z,yaw);
		run_time += dt;
	}

	void poststep(float dt)
	{
		if (run_flag)
		{
			if (run_time-ani_switch_time >= 1.0f/freq)
			{
				astro_ptr->switch_model();
				ani_switch_time = run_time;
			}
		}
		if (player)
		{
			//v = 0;
		}
		if ((!player) and (particles != nullptr))
		{
			if (disappear_flag and disappear_ani_time>0)
			{
				// cout << "update particles";
				disappear_ani_time -= dt;
				particles->Update(dt, get_move_center(), 2, glm::vec3(0), ParticleGenerator::ParticleEffect::outer2center);
				if (disappear_ani_time <=0)
				{
					particles->kill_all_particles();
				}
			}
			else if ((!disappear_flag) and appear_ani_time>0)
			{
				appear_ani_time -= dt;
				particles->Update(dt, get_move_center(), 2, glm::vec3(0), ParticleGenerator::ParticleEffect::center2outer);
				if (appear_ani_time<=0)
				{
					particles->kill_all_particles();
				}
			}
		}
	}

	BoundingSphere get_BS()
	{
		BoundingSphere BS;
		BS.center = glm::vec3(x,0,z);
		BS.R = astro_ptr->get_length()/2.0;
		return BS;
	}


	void react_collision(float dt, glm::vec3 normal)
	{
		this->step(-dt);
		
		if (!player)
		{
			glm::vec3 incid = glm::vec3(sin(yaw),0,cos(yaw));
			glm::vec3 ref = glm::reflect(incid, glm::normalize(normal));
			yaw = atan2(ref[0], ref[2]);
		}
	}

	void disappear()
	{
		disappear_flag = true;
		astro_ptr->visible = false;
		disappear_ani_time = 2.0f;
		stop();
	}

	void appear()
	{
		disappear_flag = false;
		astro_ptr->visible = true;
		appear_ani_time = 2.0f;
		start();
	}


	void stop()
	{
		run_flag = false;
		run_time = 0;
		v = 0;
		astro_ptr->geo_model = astro_ptr->still_mod;
		ani_switch_time = 0;
	}

	void start()
	{
		run_flag = true;
		run_time = 0;
		v = default_v;
		astro_ptr->geo_model = astro_ptr->move1_mod;
		ani_switch_time = 0;
	}

	MovingCenter get_move_center()
	{
		MovingCenter object;
		object.Position = glm::vec3(x, 3+astro_ptr->get_height()/2.0,z);
		object.Velocity = glm::vec3(v * sin(yaw),0, v * cos(yaw));
		return object;
	}
private:

};
