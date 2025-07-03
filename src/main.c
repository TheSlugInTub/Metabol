#include <stdio.h>
#include <caerus/caerus.h>
#include <testing.h>

int main()
{
    printf("Hello, world!");

    csSceneHandle scene = csECS_CreateScene();

    csEntityID ent = csECS_AddEntity(scene);
    CS_ECS_ASSIGN(scene, ent, Vec2);

    Vec2* comp = CS_ECS_GET(scene, ent, Vec2);

    

    return 0;
}
