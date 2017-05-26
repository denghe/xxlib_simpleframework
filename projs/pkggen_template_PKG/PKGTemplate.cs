#pragma warning disable 0169, 0414
using TemplateLibrary;

enum Color
{
    Red, Blue
}

struct Pos
{
    float x, y;
}

class SceneObj
{
    [Desc("所在场景")]
    Scene scene;
}

class Monster : SceneObj
{
    [Desc("位于 scene monster 容器中的下标 for 交换快删")]
    int scene_monsters_index;
    [CreateInstance]
    string name;
    Color color;
    Pos pos;
}

class Deamon : Monster
{
}

class Butcher : Monster
{
}

class Scene
{
    [CreateInstance]
    List<Monster> monsters;
}
