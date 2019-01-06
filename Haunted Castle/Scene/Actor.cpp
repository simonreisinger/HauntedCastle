#include "Actor.hpp"

Actor::Actor(Shader* shader)
{
	this->posz = 5;
	this->roty = 180;
	const string displayFile = "actor/actor.dae";

	init(displayFile, shader);
}

Actor::~Actor(){
	
}

void Actor::resetPosition()
{
	modelMatrix = mat4(1.0f);
}

void Actor::initActor()
{
	mat4x4 matTrans = mat4x4(1.0);
	/*
	matTrans = translate(matTrans, changeAxis * vec3(posx, posy, posz));
	matTrans = rotate(matTrans, radians(roty), changeAxis * vec3(0, 0, 1));
	*/

	PxTransform transform = PxTransform(glmToPxMat(matTrans));
	actor = Geometry::gPhysicsSDK->createRigidDynamic(transform);

	actor->setMass(100);
	actor->setRigidDynamicFlag(physx::PxRigidBodyFlag::eENABLE_CCD, false);
	actor->setLinearDamping(5);
	actor->setAngularDamping(5);
	actor->setMaxAngularVelocity(1);
	actor->setMaxDepenetrationVelocity(1);

	gScene->addActor(*actor);

	PxD6Joint* mJoint = PxD6JointCreate(*gPhysicsSDK, actor, PxTransform::createIdentity(), nullptr, actor->getGlobalPose());
	
	mJoint->setMotion(PxD6Axis::eX, PxD6Motion::eFREE); // Movement on X allowed
	mJoint->setMotion(PxD6Axis::eY, PxD6Motion::eFREE); // Movement on Y allowed
	mJoint->setMotion(PxD6Axis::eZ, PxD6Motion::eFREE); // Movement on Z allowed
	
	//This will lock a rotation around the X axis :
	mJoint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eFREE);

	//For the Y rotation axis :
	mJoint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eFREE);

	//For the Z rotation axis :
	mJoint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eFREE);
}

mat4x4 Actor::getGlobalPose()
{
	return pxMatToGlm(PxMat44(actor->getGlobalPose()));
}

void Actor::PxRotate(float rx, float ry, float rz)
{
	vec3 r = changeAxis * vec3(rx, ry, rz);
	actor->addTorque(actor->getGlobalPose().rotate(PxVec3(r.x, r.y, r.z)));
}

void Actor::PxTranslate(float tx, float ty, float tz)
{
	vec3 t = changeAxis * vec3(tx, ty, tz);
	actor->addForce(actor->getGlobalPose().rotate(PxVec3(t.x, t.y, t.z)));
}