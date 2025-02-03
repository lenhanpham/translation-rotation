## Description

The program is used to generate coordinates of molecules using two fragments in two separate xyz files. The program is useful to generate new molecular systems based on two fragments for scanning purposes. Rotation around a direction can also be done to generate new conformation. The code can only do one step of rotation. Both scanning and rotation can be done as well.



The program is at experimental step. There can be errors and bugs.



## Explanation of Each Parameter

| Parameter         | Description |
|-------------------|-------------|
| n_steps   	    | The number of steps for translation and rotation.                                                          |     
| delta_s	        | The step size for translation (distance moved per step).                                                   |   
| c	                | The minimum distance threshold for checking collisions between points.                                     |           
| angle_rot	        | The rotation angle applied at each step.                                                                   |               
| angle_unit	    | (Optional) Specifies whether angle_rot is in degrees or radians. Default is degrees.                       |               
| num_threads	    | (Optional) The number of threads to use for parallel processing. Default is the maximum available threads. |               
| output_dir	    | The directory where output files will be saved.                                                            |       
| files_set1	    | A space-separated list of .xyz files from the first set.                                                   |           
| files_set2	    | A space-separated list of .xyz files from the second set.                                                  |               
| index_1_translate	| The index of the point in files_set1 used as the reference for translation.                                |               
| index_2_translate	| The index of the point in files_set2 used as the target for translation.                                   |               
| index_1_rotate	| The index of the point in files_set1 used as the reference for rotation.                                   |       
| index_2_rotate	| The index of the point in files_set2 used as the target for rotation.                                      |               



## How to Run the Program  

Save the input parameters in a file (e.g., input_file.txt).

Make the script executable:

chmod +x configure

Run the script with required options:

./configure --eigen-dir=/path/to/eigen --prefix=/usr/local

Build the project:

make install


Run the program with the input file:
./process_xyz input_file.txt


## Input example
\# Parameters for translation and rotation

n_steps: 10                  # Number of steps for translation and rotation
delta_s: 0.5                 # Translation step size
c: 0.8                       # Minimum distance threshold
angle_rot: 30                # Rotation angle (default unit is degrees)
angle_unit: degrees          # Optional: Specify 'degrees' or 'radians' for angle_rot
num_threads: 4               # Optional: Number of threads for parallel processing
output_dir: output_xyz_files # Directory to store output files

\# Files to process

files_set1: set1_1.xyz set1_2.xyz set1_3.xyz

files_set2: set2_1.xyz set2_2.xyz set2_3.xyz

\# Indices for translation and rotation

index_1_translate: 1         # Index of the point in files_set1 for translation
index_2_translate: 2         # Index of the point in files_set2 for translation
index_1_rotate: 3            # Index of the point in files_set1 for rotation
index_2_rotate: 4            # Index of the point in files_set2 for rotation

