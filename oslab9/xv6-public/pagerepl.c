#include "types.h"
#include "stat.h"
#include "user.h"

#define MAX_FRAMES 10
#define MAX_REF 100

// Helper function to print a single step
void print_step(int page, int *frames, int num_frames, int hit) {
  printf(1, "Ref: %d | Frames: [", page);
  for(int j = 0; j < num_frames; j++) {
    if(frames[j] == -1) printf(1, " - ");
    else printf(1, " %d ", frames[j]);
  }
  printf(1, "] | %s\n", hit ? "Hit" : "Fault");
}

int fifo(int *ref, int ref_len, int num_frames, int print_details) {
  int frames[MAX_FRAMES];
  for(int i = 0; i < num_frames; i++) frames[i] = -1;
  int faults = 0;
  int replace_idx = 0;

  for(int i = 0; i < ref_len; i++) {
    int page = ref[i];
    int hit = 0;
    
    for(int j = 0; j < num_frames; j++) {
      if(frames[j] == page) { hit = 1; break; }
    }
    
    if(!hit) {
      frames[replace_idx] = page;
      replace_idx = (replace_idx + 1) % num_frames;
      faults++;
    }
    
    if(print_details) print_step(page, frames, num_frames, hit);
  }
  return faults;
}

int lru(int *ref, int ref_len, int num_frames, int print_details) {
  int frames[MAX_FRAMES];
  int last_used[MAX_FRAMES];
  for(int i = 0; i < num_frames; i++) { frames[i] = -1; last_used[i] = 0; }
  int faults = 0;
  int time = 0;

  for(int i = 0; i < ref_len; i++) {
    time++;
    int page = ref[i];
    int hit = 0;
    int hit_idx = -1;
    
    for(int j = 0; j < num_frames; j++) {
      if(frames[j] == page) { hit = 1; hit_idx = j; break; }
    }
    
    if(hit) {
      last_used[hit_idx] = time;
    } else {
      faults++;
      int lru_idx = 0;
      int min_time = time + 1;
      
      for(int j = 0; j < num_frames; j++) {
        if(frames[j] == -1) { lru_idx = j; break; }
        if(last_used[j] < min_time) { min_time = last_used[j]; lru_idx = j; }
      }
      frames[lru_idx] = page;
      last_used[lru_idx] = time;
    }
    
    if(print_details) print_step(page, frames, num_frames, hit);
  }
  return faults;
}

int main(int argc, char *argv[]) {
  // If the user provides arguments: pagerepl <frames> <ref1> <ref2> ...
  if (argc > 2) {
    int custom_ref[MAX_REF];
    int custom_len = 0;
    int custom_frames = atoi(argv[1]);
    
    for(int i = 2; i < argc && custom_len < MAX_REF; i++) {
      custom_ref[custom_len++] = atoi(argv[i]);
    }
    
    printf(1, "--- Custom Run: %d frames ---\n", custom_frames);
    printf(1, "FIFO:\n");
    fifo(custom_ref, custom_len, custom_frames, 1);
    printf(1, "\nLRU:\n");
    lru(custom_ref, custom_len, custom_frames, 1);
    exit();
  }

  // Default execution to satisfy assignment requirements Tasks (b) and (c)
  int ref[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
  int ref_len = sizeof(ref) / sizeof(ref[0]);
  
  printf(1, "--- Task (b): FIFO with 3 frames ---\n");
  int fifo_3 = fifo(ref, ref_len, 3, 1);
  printf(1, "Total FIFO Faults: %d\n", fifo_3);
  
  printf(1, "\n--- Task (b): LRU with 3 frames ---\n");
  int lru_3 = lru(ref, ref_len, 3, 1);
  printf(1, "Total LRU Faults: %d\n", lru_3);
  
  printf(1, "\n--- Task (c): Comparison Table ---\n");
  printf(1, "Frames | FIFO Faults | FIFO Hit Ratio | LRU Faults | LRU Hit Ratio\n");
  printf(1, "------------------------------------------------------------------\n");
  
  for(int f = 3; f <= 5; f++) {
    int fifo_f = fifo(ref, ref_len, f, 0);
    int lru_f = lru(ref, ref_len, f, 0);
    
    // Calculate hit ratio as a percentage using integer math
    int fifo_hit_pct = ((ref_len - fifo_f) * 100) / ref_len;
    int lru_hit_pct = ((ref_len - lru_f) * 100) / ref_len;
    
    printf(1, "  %d    |     %d      |      %d%%       |     %d     |      %d%%\n", 
           f, fifo_f, fifo_hit_pct, lru_f, lru_hit_pct);
  }
  
  exit();
}
