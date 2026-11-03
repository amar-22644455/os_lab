#include "types.h"
#include "stat.h"
#include "user.h"

#define REF_LEN 20

// The reference string provided in the assignment
int ref_str[REF_LEN] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};

void run_fifo(int num_frames, int verbose) {
    int frames[5];
    for(int i=0; i<num_frames; i++) frames[i] = -1;
    int head = 0, hits = 0, faults = 0;

    if(verbose) printf(1, "\n--- FIFO (%d Frames) ---\n", num_frames);
    
    for(int i = 0; i < REF_LEN; i++) {
        int page = ref_str[i];
        int found = 0;
        for(int j = 0; j < num_frames; j++) {
            if(frames[j] == page) { found = 1; break; }
        }
        
        if(found) {
            hits++;
            if(verbose) {
                printf(1, "Page %d: Hit   | Frames: ", page);
                for(int j=0; j<num_frames; j++) frames[j] == -1 ? printf(1, "[ ] ") : printf(1, "[%d] ", frames[j]);
                printf(1, "\n");
            }
        } else {
            faults++;
            frames[head] = page;
            head = (head + 1) % num_frames;
            if(verbose) {
                printf(1, "Page %d: FAULT | Frames: ", page);
                for(int j=0; j<num_frames; j++) frames[j] == -1 ? printf(1, "[ ] ") : printf(1, "[%d] ", frames[j]);
                printf(1, "\n");
            }
        }
    }
    if(!verbose) {
        int hit_pct = (hits * 100) / REF_LEN;
        int flt_pct = (faults * 100) / REF_LEN;
        printf(1, "FIFO    | %d      | %d      | %d    | %d%%       | %d%%\n", num_frames, faults, hits, hit_pct, flt_pct);
    }
}

void run_lru(int num_frames, int verbose) {
    int frames[5], last_used[5];
    for(int i=0; i<num_frames; i++) { frames[i] = -1; last_used[i] = 0; }
    int hits = 0, faults = 0, time = 0;

    if(verbose) printf(1, "\n--- LRU (%d Frames) ---\n", num_frames);
    
    for(int i = 0; i < REF_LEN; i++) {
        int page = ref_str[i];
        time++;
        int found = 0, pos = -1;
        
        for(int j = 0; j < num_frames; j++) {
            if(frames[j] == page) { found = 1; pos = j; break; }
        }
        
        if(found) {
            hits++;
            last_used[pos] = time;
            if(verbose) {
                printf(1, "Page %d: Hit   | Frames: ", page);
                for(int j=0; j<num_frames; j++) frames[j] == -1 ? printf(1, "[ ] ") : printf(1, "[%d] ", frames[j]);
                printf(1, "\n");
            }
        } else {
            faults++;
            int replace_idx = -1;
            for(int j = 0; j < num_frames; j++) {
                if(frames[j] == -1) { replace_idx = j; break; }
            }
            if(replace_idx == -1) {
                int min_time = time + 1;
                for(int j = 0; j < num_frames; j++) {
                    if(last_used[j] < min_time) { min_time = last_used[j]; replace_idx = j; }
                }
            }
            frames[replace_idx] = page;
            last_used[replace_idx] = time;
            if(verbose) {
                printf(1, "Page %d: FAULT | Frames: ", page);
                for(int j=0; j<num_frames; j++) frames[j] == -1 ? printf(1, "[ ] ") : printf(1, "[%d] ", frames[j]);
                printf(1, "\n");
            }
        }
    }
    if(!verbose) {
        int hit_pct = (hits * 100) / REF_LEN;
        int flt_pct = (faults * 100) / REF_LEN;
        printf(1, "LRU     | %d      | %d      | %d    | %d%%       | %d%%\n", num_frames, faults, hits, hit_pct, flt_pct);
    }
}

void run_opt(int num_frames, int verbose) {
    int frames[5];
    for(int i=0; i<num_frames; i++) frames[i] = -1;
    int hits = 0, faults = 0;

    if(verbose) printf(1, "\n--- Optimal (%d Frames) ---\n", num_frames);
    
    for(int i = 0; i < REF_LEN; i++) {
        int page = ref_str[i];
        int found = 0;
        
        for(int j = 0; j < num_frames; j++) {
            if(frames[j] == page) { found = 1; break; }
        }
        
        if(found) {
            hits++;
            if(verbose) {
                printf(1, "Page %d: Hit   | Frames: ", page);
                for(int j=0; j<num_frames; j++) frames[j] == -1 ? printf(1, "[ ] ") : printf(1, "[%d] ", frames[j]);
                printf(1, "\n");
            }
        } else {
            faults++;
            int replace_idx = -1;
            for(int j = 0; j < num_frames; j++) {
                if(frames[j] == -1) { replace_idx = j; break; }
            }
            
            if(replace_idx == -1) {
                int farthest = -1;
                for(int j = 0; j < num_frames; j++) {
                    int next_use = REF_LEN + 1;
                    for(int k = i + 1; k < REF_LEN; k++) {
                        if(ref_str[k] == frames[j]) { next_use = k; break; }
                    }
                    if(next_use > farthest) { farthest = next_use; replace_idx = j; }
                }
            }
            frames[replace_idx] = page;
            if(verbose) {
                printf(1, "Page %d: FAULT | Frames: ", page);
                for(int j=0; j<num_frames; j++) frames[j] == -1 ? printf(1, "[ ] ") : printf(1, "[%d] ", frames[j]);
                printf(1, "\n");
            }
        }
    }
    if(!verbose) {
        int hit_pct = (hits * 100) / REF_LEN;
        int flt_pct = (faults * 100) / REF_LEN;
        printf(1, "Optimal | %d      | %d      | %d    | %d%%       | %d%%\n", num_frames, faults, hits, hit_pct, flt_pct);
    }
}

int main(void) {
    // Task 4(a) and 4(b): Display step-by-step for 3 frames
    run_fifo(3, 1);
    run_lru(3, 1);
    run_opt(3, 1);

    // Task 4(c): Summary Comparison Table for 3, 4, 5 frames
    printf(1, "\n=========================================================\n");
    printf(1, "Algo    | Frames | Faults | Hits | Hit Ratio | Fault Ratio\n");
    printf(1, "---------------------------------------------------------\n");
    run_fifo(3, 0); run_fifo(4, 0); run_fifo(5, 0);
    printf(1, "---------------------------------------------------------\n");
    run_lru(3, 0); run_lru(4, 0); run_lru(5, 0);
    printf(1, "---------------------------------------------------------\n");
    run_opt(3, 0); run_opt(4, 0); run_opt(5, 0);
    printf(1, "=========================================================\n");

    printf(1, "\nConclusion: \n");
    printf(1, "- Optimal performs best with the fewest page faults because it looks into the future.\n");
    printf(1, "- LRU performs better than FIFO by utilizing locality of reference (past usage).\n");
    printf(1, "- Increasing frames from 3 to 5 decreases total faults across all algorithms.\n\n");

    exit();
}
