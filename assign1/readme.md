# Assignment 1
## Team Members
|std_id|Name|
|--------|-|
|k21-3084|Muneer Raza|
|k21-4739|Zainab Irshad|
|k21-3076|Abdul Rafay|
## Output Screenshots
<img src="images/output-1.jpg" />
<hr />
<img src="images/output-2.jpg" />
<hr />
<img src="images/output-3.jpg" />
<hr />
<img src="images/output-4.jpg" />
<hr />
<img src="images/output-5.jpg" />
<hr />
<img src="images/result.png" />

## Results and Analysis
<img src="images/average_time.png" />
<img src="images/input_64_time.png" />
<img src="images/input_128_time.png" />
<img src="images/input_256_time.png" />
<img src="images/input_512_time.png" />
<img src="images/input_1024_time.png" />
<img src="images/input_2048_time.png" />
<img src="images/input_4096_time.png" />
<img src="images/input_8192_time.png" />
<img src="images/input_16384_time.png" />
<img src="images/input_32768_time.png" />

## Major Problems Encountered
1. **Issue 1:** 
    - In the MPI tiled program, initially, we encountered a problem with `mpi_scatter` as it divided the data equally among processors, leading to incorrect results because each processor required the entire vector. To address this issue, we implemented `mpi_bcast`, which effectively resolved the problem by ensuring that each processor received the complete vector.
    - **Resolved**

3. **Issue 2:**
    - We encountered an issue with the MPI initialization function being called before passing command-line arguments. This misplacement resulted in an error where the program terminated normally, but one process returned a non-zero exit code, causing the job to be aborted. Additionally, process rank 1 on node DESKTOP-P9UPIHE exited due to a segmentation fault (signal 11). To rectify this issue, we repositioned the `MPI initialization` function after passing the command-line arguments, resolving the segmentation fault problem.
    - **Resolved**
