/** Setup function for ym blep synthesis engine.
 *
 *    The ym_blep_setup() function sets ym blep synthesis engine for
 *    this ym emulator instance.
 *
 *  @parm    ym  ym emulator instance to setup
 *  @retval   0  on success
 *  @retval  -1  on failure
 */
int ym_blep_setup(ym_t * const ym);

enum {
    MAX_BLEPS = 512
};

typedef struct {
    int event;
    int count;
    int flip_flop;
} ym_blep_tone_t;

typedef struct {
    int stamp;
    int level;
} ym_blep_blep_state_t;

typedef struct {
    /* sampling parameters */
    int cycles_per_sample; /* 8 bit fixed point ym clocks */
    int cycles_to_next_sample;

    /* hp filter */
    int hp;

    /* blep stuff */
    int global_output_level;
    int blep_idx;
    int systemtime;
    ym_blep_blep_state_t blepstate[MAX_BLEPS];

    /* subsystem states */
    ym_blep_tone_t tonegen[3];
    int noise_event;
    int noise_count;
    int noise_state;
    int noise_output;

    int env_event;
    int env_count;
    int env_state;
    int env_output;

} ym_blep_t;
