#include "Debug.hpp"
#include "Shader.hpp"

class GameObject
{
public:
	GameObject();
	~GameObject();
	
	//
	void Render();

private:
	Shader shader;
};	