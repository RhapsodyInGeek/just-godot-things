/*****************************************************
INTERMISSION CAMERA
When the stage ends, one of these is randomly chosen
as the new viewport camera and looks around.
******************************************************/
class IntermissionCam : public Node3D
{
    GDCLASS(IntermissionCam, Node3D)
private:
    static void _bind_methods();
public:
    GameManager* GAME;
    Camera3D* cam;
    Dictionary properties = {};
    void set_properties(Dictionary p);
    Dictionary get_properties();
    IntermissionCam();
    ~IntermissionCam();
    void _ready();
    void _process(float delta);
};