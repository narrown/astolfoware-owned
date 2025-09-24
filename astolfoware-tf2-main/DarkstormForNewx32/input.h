#pragma once
#include "SDKClasses.h"
#include "bitbuf.h"
#include <Windows.h>

class IDoNotCareNigga;
enum kbutton_t;
enum button_code_t;
enum game_action_set_t;
enum camera_third_data_t;
enum game_action_set_flags_t;

class CInput {
public:
	virtual	void init_all(void) = 0;
	virtual void shutdown_all(void) = 0;
	virtual int get_button_bits(int) = 0;
	virtual void create_move(int sequence_number, float input_sample_frametime, bool active) = 0; // 3
	virtual void extra_mouse_sample(float frametime, bool active) = 0; // 4 
	virtual bool write_usercmd_delta_to_buffer(bf_write* buf, int from, int to, bool isnewcommand) = 0; // 5
	virtual void encode_user_cmd_to_buffer(bf_write& buf, int slot) = 0; // 6
	virtual void decode_user_cmd_from_buffer(bf_read& buf, int slot) = 0; // 7
	virtual CUserCmd* get_user_cmd(int sequence_number) = 0; // 8
	virtual void MakeWeaponSelection(IDoNotCareNigga* weapon) = 0;
	virtual float key_state(kbutton_t* key) = 0;
	virtual int key_event(int eventcode, button_code_t keynum, const char* current_binding) = 0;
	virtual kbutton_t* find_key(const char* name) = 0;
	virtual void controller_commands(void) = 0;
	virtual void joystick_advanced(void) = 0;
	virtual void joystick_set_sample_time(float frametime) = 0;
	virtual void in_set_sample_time(float frametime) = 0;
	virtual void accumulate_mouse(void) = 0;
	virtual void activate_mouse(void) = 0;
	virtual void deactivate_mouse(void) = 0;
	virtual void clear_states(void) = 0;
	virtual float get_look_spring(void) = 0;
	virtual void get_fullscreen_mouse_pos(int* mx, int* my, int* unclampedx = 0, int* unclampedy = 0) = 0;
	virtual void set_fullscreen_mouse_pos(int mx, int my) = 0;
	virtual void reset_mouse(void) = 0;
	virtual	float get_last_forward_move(void) = 0;
	virtual	float joystick_get_forward(void) = 0;
	virtual	float joystick_get_side(void) = 0;
	virtual	float joystick_get_pitch(void) = 0;
	virtual	float joystick_get_yaw(void) = 0;
	virtual void cam_think(void) = 0;
	virtual int cam_is_third_person(void) = 0;
	virtual void cam_to_third_person(void) = 0;
	virtual void cam_to_first_person(void) = 0;
	virtual void cam_start_mouse_move(void) = 0;
	virtual void cam_end_mouse_move(void) = 0;
	virtual void cam_start_distance(void) = 0;
	virtual void cam_end_distance(void) = 0;
	virtual int	cam_intercepting_mouse(void) = 0;
	virtual void cam_to_orthographic() = 0;
	virtual	bool cam_is_orthographic() const = 0;
	virtual	void cam_orthographic_size(float& w, float& h) const = 0;
	virtual void set_preferred_game_action_set(game_action_set_t action_set) = 0;
	virtual game_action_set_t get_preferred_game_action_set() = 0;
	virtual void set_game_action_set_flags(game_action_set_flags_t action_set_flags) = 0;
	virtual void level_init(void) = 0;
	virtual void clear_input_button(int bits) = 0;
	virtual	void cam_set_camera_third_data(camera_third_data_t* camera_data, const Vector& camera_offset) = 0;
	virtual void cam_camera_third_think(void) = 0;
	virtual	bool enable_joystick_mode() = 0;
	virtual bool is_steam_controller_active() = 0;
	CUserCmd* GetCommands() {
		return *reinterpret_cast<CUserCmd**>(reinterpret_cast<DWORD>(this) + 0xFC);
	}
};