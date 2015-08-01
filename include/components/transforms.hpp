#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

#include "entity.hpp"

#include "../proto/components.pb.h"

namespace tec {
	static glm::vec3 FORWARD_VECTOR(0.0f, 0.0f, -1.0f);
	static glm::vec3 UP_VECTOR(0.0f, 1.0f, 0.0f);
	static glm::vec3 RIGHT_VECTOR(1.0f, 0.0f, 0.0f);

	struct Position {
		Position(glm::vec3 pos) : value(pos) { }
		Position() { }
		// Translates by amount.
		void Translate(const glm::vec3 amount);

		// Translates by amount in direction orientation.
		void Translate(const glm::vec3 amount, const glm::quat orientation);

		glm::vec3 value;

		void Out(proto::Component* target);
		void In(const proto::Component& source);

		static ReflectionComponent Reflection(Position* val) {
			ReflectionComponent refcomp;
			Property prop(Property::FLOAT);
			(refcomp.properties["x"] = prop).Set(val->value.x);
			refcomp.properties["x"].update_func = [val] (Property& prop) {
				val->value.x = prop.Get<float>();
			};
			(refcomp.properties["y"] = prop).Set(val->value.y);
			refcomp.properties["y"].update_func = [val] (Property& prop) {
				val->value.y = prop.Get<float>();
			};
			(refcomp.properties["z"] = prop).Set(val->value.z);
			refcomp.properties["z"].update_func = [val] (Property& prop) {
				val->value.z = prop.Get<float>();
			};
			return std::move(refcomp);
		}
	};

	struct Orientation {
		Orientation(glm::vec3 rot) {
			Rotate(rot);
		}
		Orientation(glm::quat q) : value(q), rotation(glm::eulerAngles(q)) { }
		Orientation() { }
		void Rotate(const glm::vec3 amount);

		// Adds the rotation amount based on the current orientation.
		void OrientedRotate(const glm::vec3 amount);

		glm::quat value;
		glm::vec3 rotation;

		void Out(proto::Component* target);
		void In(const proto::Component& source);

		static ReflectionComponent Reflection(Orientation* val) {
			ReflectionComponent refcomp;
			Property prop(Property::FLOAT);
			(refcomp.properties["x"] = prop).Set(val->value.x);
			refcomp.properties["x"].update_func = [val] (Property& prop) {
				val->value.x = prop.Get<float>();
			};
			(refcomp.properties["y"] = prop).Set(val->value.y);
			refcomp.properties["y"].update_func = [val] (Property& prop) {
				val->value.y = prop.Get<float>();
			};
			(refcomp.properties["z"] = prop).Set(val->value.z);
			refcomp.properties["z"].update_func = [val] (Property& prop) {
				val->value.z = prop.Get<float>();
			};
			(refcomp.properties["w"] = prop).Set(val->value.w);
			refcomp.properties["w"].update_func = [val] (Property& prop) {
				val->value.w = prop.Get<float>();
			};
			return std::move(refcomp);
		}
	};

	struct Scale {
		Scale(glm::vec3 scale) : value(scale) { }
		Scale() { }

		glm::vec3 value;

		void Out(proto::Component* target);
		void In(const proto::Component& source);

		static ReflectionComponent Reflection(Scale* val) {
			ReflectionComponent refcomp;
			Property prop(Property::FLOAT);
			(refcomp.properties["x"] = prop).Set(val->value.x);
			refcomp.properties["x"].update_func = [val] (Property& prop) {
				val->value.x = prop.Get<float>();
			};
			(refcomp.properties["y"] = prop).Set(val->value.y);
			refcomp.properties["y"].update_func = [val] (Property& prop) {
				val->value.y = prop.Get<float>();
			};
			(refcomp.properties["z"] = prop).Set(val->value.z);
			refcomp.properties["z"].update_func = [val] (Property& prop) {
				val->value.z = prop.Get<float>();
			};
			return std::move(refcomp);
		}
	};
}
