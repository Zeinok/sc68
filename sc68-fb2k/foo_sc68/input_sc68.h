#pragma once

#define dbg(sc68,fmt,...)

class input_sc68 {

public:
	char m_name[16];
	sc68_t * m_sc68;
	sc68_minfo_t m_fileinfo;
	int m_sampling_rate;

	static int g_counter;
	static volatile LONG g_instance;

	input_sc68();
	~input_sc68();
	void open(service_ptr_t<file>, const char *, t_input_open_reason, abort_callback &);
	unsigned get_subsong_count();
	t_uint32 input_sc68::get_subsong(unsigned);
	void get_info(t_uint32, file_info &, abort_callback &);
	t_filestats get_file_stats(abort_callback & p_abort);
	void decode_initialize(t_uint32,unsigned,abort_callback &);
	bool decode_run(audio_chunk &,abort_callback &);
	void input_sc68::decode_seek(double,abort_callback &);
	bool decode_can_seek();
	bool decode_get_dynamic_info(file_info &, double &);
	bool decode_get_dynamic_info_track(file_info &, double &);
	void decode_on_idle(abort_callback &);
	void retag_set_info(t_uint32,const file_info &,abort_callback &);
	void retag_commit(abort_callback &);
	static bool g_is_our_content_type(const char *);
	static bool g_is_our_path(const char *,const char *);
};

struct cmenu_def {
	const GUID * guid;
	const char * name;
	const char * desc;
};

extern const struct cmenu_def asid_defs[];
extern const struct cmenu_def engine_defs[];
extern volatile int g_ym_engine;
extern volatile int g_ym_filter;
extern volatile int g_ym_asid;
