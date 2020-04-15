
#include "player.h"

#include "inputmaster.h"
#include "uimaster.h"

using namespace LucKey;

InputMaster::InputMaster(Context* context):
    Object(context)
{
    keyBindingsMaster_[KEY_UP]     = buttonBindingsMaster_[static_cast<int>(SixaxisButton::SB_DPAD_UP)]    = MasterInputAction::UP;
    keyBindingsMaster_[KEY_DOWN]   = buttonBindingsMaster_[static_cast<int>(SixaxisButton::SB_DPAD_DOWN)]  = MasterInputAction::DOWN;
    keyBindingsMaster_[KEY_LEFT]   = buttonBindingsMaster_[static_cast<int>(SixaxisButton::SB_DPAD_LEFT)]  = MasterInputAction::LEFT;
    keyBindingsMaster_[KEY_RIGHT]  = buttonBindingsMaster_[static_cast<int>(SixaxisButton::SB_DPAD_RIGHT)] = MasterInputAction::RIGHT;
    keyBindingsMaster_[KEY_RETURN] = buttonBindingsMaster_[static_cast<int>(SixaxisButton::SB_CROSS)]      = MasterInputAction::CONFIRM;
    keyBindingsMaster_[KEY_ESCAPE] = buttonBindingsMaster_[static_cast<int>(SixaxisButton::SB_CIRCLE)]     = MasterInputAction::CANCEL;
    keyBindingsMaster_[KEY_P]      = buttonBindingsMaster_[static_cast<int>(SixaxisButton::SB_START)]      = MasterInputAction::PAUSE;
    keyBindingsMaster_[KEY_ESCAPE] = MasterInputAction::MENU;

    keyBindingsPlayer_[1][KEY_W]      = PlayerInputAction::MOVE_UP;
    keyBindingsPlayer_[1][KEY_S]      = PlayerInputAction::MOVE_DOWN;
    keyBindingsPlayer_[1][KEY_A]      = PlayerInputAction::MOVE_LEFT;
    keyBindingsPlayer_[1][KEY_D]      = PlayerInputAction::MOVE_RIGHT;
    keyBindingsPlayer_[1][KEY_C]      = PlayerInputAction::DASH;
    keyBindingsPlayer_[1][KEY_V]      = PlayerInputAction::FIRE;
    keyBindingsPlayer_[1][KEY_KP_8]   = PlayerInputAction::FIRE_N;
    keyBindingsPlayer_[1][KEY_KP_5]   = PlayerInputAction::FIRE_S;
    keyBindingsPlayer_[1][KEY_KP_2]   = PlayerInputAction::FIRE_S;
    keyBindingsPlayer_[1][KEY_KP_4]   = PlayerInputAction::FIRE_W;
    keyBindingsPlayer_[1][KEY_KP_6]   = PlayerInputAction::FIRE_E;
    keyBindingsPlayer_[1][KEY_KP_9]   = PlayerInputAction::FIRE_NE;
    keyBindingsPlayer_[1][KEY_KP_3]   = PlayerInputAction::FIRE_SE;
    keyBindingsPlayer_[1][KEY_KP_1]   = PlayerInputAction::FIRE_SW;
    keyBindingsPlayer_[1][KEY_KP_7]   = PlayerInputAction::FIRE_NW;

    keyBindingsPlayer_[2][KEY_UP]     = PlayerInputAction::MOVE_UP;
    keyBindingsPlayer_[2][KEY_DOWN]   = PlayerInputAction::MOVE_DOWN;
    keyBindingsPlayer_[2][KEY_LEFT]   = PlayerInputAction::MOVE_LEFT;
    keyBindingsPlayer_[2][KEY_RIGHT]  = PlayerInputAction::MOVE_RIGHT;
    keyBindingsPlayer_[2][KEY_LSHIFT] = PlayerInputAction::DASH;

    SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(InputMaster, HandleKeyDown));
    SubscribeToEvent(E_KEYUP, URHO3D_HANDLER(InputMaster, HandleKeyUp));
    SubscribeToEvent(E_JOYSTICKBUTTONDOWN, URHO3D_HANDLER(InputMaster, HandleJoystickButtonDown));
    SubscribeToEvent(E_JOYSTICKBUTTONUP, URHO3D_HANDLER(InputMaster, HandleJoystickButtonUp));
    SubscribeToEvent(E_JOYSTICKAXISMOVE, URHO3D_HANDLER(InputMaster, HandleJoystickAxisMove));
    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(InputMaster, HandleUpdate));
}

void InputMaster::HandleUpdate(StringHash eventType, VariantMap &eventData)
{ (void)eventType; (void)eventData;

    InputActions activeActions{};
    InputActions keyupActions{};

    for (Player* p : MC->GetPlayers()){

        int pId{ p->GetPlayerId() };
        Vector<PlayerInputAction> emptyActions{};
        activeActions.player_[pId] = emptyActions;
    }

    //Convert key presses to actions
    for (int key : pressedKeys_){
        //Check for master key presses
        if (keyBindingsMaster_.Contains(key)){
            MasterInputAction action{keyBindingsMaster_[key]};
            if (!activeActions.master_.Contains(action))
                activeActions.master_.Push(action);
        }
        //Check for player key presses
        for (Player* p : MC->GetPlayers()){

            int pId{ p->GetPlayerId() };
            if (keyBindingsPlayer_[pId].Contains(key)){
                PlayerInputAction action{keyBindingsPlayer_[pId][key]};
                if (!activeActions.player_[pId].Contains(action))
                    activeActions.player_[pId].Push(action);
            }
        }
    }


    //Convert keyUp releases to actions
    for (int key : keyUpKeys_){
        //Check for master key presses
        if (keyBindingsMaster_.Contains(key)){
            keyUpKeys_.Remove(key); // TODO: removing inside loop!!
            MasterInputAction action{keyBindingsMaster_[key]};
            if (!keyupActions.master_.Contains(action))
                keyupActions.master_.Push(action);
        }
    }

    //Check for joystick button presses
    for (Player* p : MC->GetPlayers()){

        int pId{ p->GetPlayerId() };
        for (int button : pressedJoystickButtons_[pId-1])
            if (buttonBindingsPlayer_[pId].Contains(button)){
                PlayerInputAction action{ buttonBindingsPlayer_[pId][button]};
                if (!activeActions.player_[pId].Contains(action))
                    activeActions.player_[pId].Push(action);
            }
    }

    //Handle the registered actions
    HandleActions(activeActions, keyupActions);
}

void InputMaster::HandleActions(const InputActions& actions, const InputActions& keyUpActions)
{
    //Handle master actions
    for (MasterInputAction action : actions.master_){
        switch (action){
        case MasterInputAction::UP:
            break;
        case MasterInputAction::DOWN:
            break;
        case MasterInputAction::LEFT:               break;
        case MasterInputAction::RIGHT:              break;
        case MasterInputAction::CONFIRM:            break;
        case MasterInputAction::CANCEL:             break;
        case MasterInputAction::PAUSE:              break;
        case MasterInputAction::MENU:               break;
        default: break;
        }
    }

    for (MasterInputAction action : keyUpActions.master_){
        switch (action){
        case MasterInputAction::UP:
        case MasterInputAction::LEFT:
            GetSubsystem<UIMaster>()->Next();
            break;
        case MasterInputAction::DOWN:
        case MasterInputAction::RIGHT:
            GetSubsystem<UIMaster>()->Previous();
            break;
        case MasterInputAction::CONFIRM:
            GetSubsystem<UIMaster>()->Invoke();
            break;
        case MasterInputAction::CANCEL:             break;
        case MasterInputAction::PAUSE:              break;
        case MasterInputAction::MENU:               break;
        default: break;
        }
    }

    //Handle player actions
    for (Player* p : MC->GetPlayers()){

        int pId{ p->GetPlayerId() };
        auto playerInputActions = actions.player_[pId];

        Controllable* controlled{ controlledByPlayer_[pId] };
        if (controlled){

            Vector3 stickMove{ Vector3(axesPosition_[pId-1][0], 0.0f, -axesPosition_[pId-1][1]) };
            Vector3 stickAim{  Vector3(axesPosition_[pId-1][2], 0.0f, -axesPosition_[pId-1][3]) };

            controlled->SetMove(GetMoveFromActions(playerInputActions) + stickMove);
            controlled->SetAim(GetAimFromActions(playerInputActions) + stickAim);

            std::bitset<4>restActions{};
            restActions[0] = playerInputActions->Contains(PlayerInputAction::DASH);
            restActions[1] = playerInputActions->Contains(PlayerInputAction::FIRE);

            controlled->SetActions(restActions);
        }
    }
}

void InputMaster::Screenshot()
{
    Image screenshot(context_);
    Graphics* graphics{ GetSubsystem<Graphics>() };
    graphics->TakeScreenShot(screenshot);
    //Here we save in the Data folder with date and time appended
    String fileName{ GetSubsystem<FileSystem>()->GetProgramDir() + "Screenshots/Screenshot_" +
                Time::GetTimeStamp().Replaced(':', '_').Replaced('.', '_').Replaced(' ', '_')+".png" };
    //Log::Write(1, fileName);
    screenshot.SavePNG(fileName);
}

void InputMaster::HandleKeyDown(StringHash eventType, VariantMap &eventData)
{
    int key{ eventData[KeyDown::P_KEY].GetInt() };
    if (!pressedKeys_.Contains(key)) pressedKeys_.Push(key);

    switch (key){
    case KEY_ESCAPE:{
        ErrorExit("Exitting ... ", EXIT_SUCCESS);
        MC->Exit();
    } break;
    case KEY_9:{
        Screenshot();
    } break;
    default: break;
    }
}

void InputMaster::HandleKeyUp(StringHash eventType, VariantMap &eventData)
{ (void)eventType;

    int key{ eventData[KeyUp::P_KEY].GetInt() };
    if (pressedKeys_.Contains(key))  {
        pressedKeys_.Remove(key);
        keyUpKeys_.Push(key);
    }
}

void InputMaster::HandleJoystickButtonDown(StringHash eventType, VariantMap &eventData)
{ (void)eventType;

    int joystickId{ eventData[JoystickButtonDown::P_JOYSTICKID].GetInt() };
    SixaxisButton button{ static_cast<SixaxisButton>(eventData[JoystickButtonDown::P_BUTTON].GetInt()) };
    if (!pressedJoystickButtons_[joystickId].Contains(button)) pressedJoystickButtons_[joystickId].Push(button);
}
void InputMaster::HandleJoystickButtonUp(StringHash eventType, VariantMap &eventData)
{ (void)eventType;

    int joystickId{ eventData[JoystickButtonDown::P_JOYSTICKID].GetInt() };
    SixaxisButton button{ static_cast<SixaxisButton>(eventData[JoystickButtonUp::P_BUTTON].GetInt()) };
    if (pressedJoystickButtons_[joystickId].Contains(button)) pressedJoystickButtons_[joystickId].Remove(button);
}

void InputMaster::HandleJoystickAxisMove(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{ (void)eventType;

    int joystickId{ eventData[JoystickAxisMove::P_JOYSTICKID].GetInt() };
    int axis{ eventData[JoystickAxisMove::P_AXIS].GetInt() };
    float position{ eventData[JoystickAxisMove::P_POSITION].GetFloat() };

    axesPosition_[joystickId][axis] = position;
}

Vector3 InputMaster::GetMoveFromActions(Vector<PlayerInputAction>* actions)
{
    return Vector3{Vector3::RIGHT *
                (actions->Contains(PlayerInputAction::MOVE_RIGHT) -
                 actions->Contains(PlayerInputAction::MOVE_LEFT))

                + Vector3::FORWARD *
                (actions->Contains(PlayerInputAction::MOVE_UP) -
                 actions->Contains(PlayerInputAction::MOVE_DOWN))};
}
Vector3 InputMaster::GetAimFromActions(Vector<PlayerInputAction>* actions)
{
    return Vector3{ Vector3::RIGHT *
                (actions->Contains(PlayerInputAction::FIRE_E) -
                 actions->Contains(PlayerInputAction::FIRE_W))

                +   Vector3::FORWARD *
                (actions->Contains(PlayerInputAction::FIRE_N) -
                 actions->Contains(PlayerInputAction::FIRE_S))
                + Quaternion(45.0f, Vector3::UP) *
                (Vector3::RIGHT *
                 (actions->Contains(PlayerInputAction::FIRE_SE) -
                  actions->Contains(PlayerInputAction::FIRE_NW))

                 +   Vector3::FORWARD *
                 (actions->Contains(PlayerInputAction::FIRE_NE) -
                  actions->Contains(PlayerInputAction::FIRE_SW)))};
}

void InputMaster::SetPlayerControl(Player* player, Controllable* controllable)
{
    int playerId{ player->GetPlayerId() };

    if (controlledByPlayer_.Contains(playerId)){
        if (controlledByPlayer_[playerId] == controllable)
            return;
        controlledByPlayer_[playerId]->ClearControl();
    }
    controlledByPlayer_[playerId] = controllable;
}

Player* InputMaster::GetPlayerByControllable(Controllable* controllable)
{
    for (int k : controlledByPlayer_.Keys())
    {
        if (controlledByPlayer_[k] == controllable)
            return MC->GetPlayer(k);
    }
    return nullptr;
}
Controllable* InputMaster::GetControllableByPlayer(int playerId)
{

    return controlledByPlayer_[playerId];
}
