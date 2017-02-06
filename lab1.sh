#!/bin/bash                                                                                          

############################################################################
# Lab LinuxInfoT2
# Brother Jones, CS 345, Operating Systems
# Preston Price
############################################################################

############################################################################                         
#                                                                                                    
# Changes made to my code for the re-submission:                                                     
#   (examples:)
#   - I edited the output for how long since the system has been booted.
#                                                                                                    
#                                                                                                    
############################################################################                        
#                                                                                                     
############################################################################                         
# Summary:                                                                                           
#    Outputs information about the current system
#                                                                                                    
#                                                                                                     
############################################################################  

# Get CPU type and model from /proc/cpuinfo
echo "What is the CPU type and model?"
output=$(grep -m 1 'model name' /proc/cpuinfo | cut -c 14-) 
printf "    %s\n\n" "$output"

# Check how many processors are on the system according to /proc/cpuinfo
echo "How many processors on this system?"
output=$(grep -o processor /proc/cpuinfo | wc -l)
printf "    %s processors\n\n" "$output"

# Check what Linux kernel is being used using uname
echo "What version of the Linux kernel is being used?"
echo -e "    "$(uname -s) version $(uname -r) $(uname -v)"\n"

# See how long since the last system reboot using uptime
echo "How long has it been since the system was last booted?"
output=$(uptime | cut -c 11-26)
echo -e "    "$output"\n" | sed 's/,/, /g'

# View stats on CPU time in /proc/stat
echo "How much of the total CPU time available (across all processors) has been spent executing in User mode? System mode? Idle?"
str=$(less /proc/stat | grep -m 1 cpu | cut -c 4-)
array=(${str//:/ })
user_time=$(echo "scale=2; (${array[0]} + ${array[1]}) / 100" | bc -l)
sys_time=$(echo "scale=2; ${array[2]} / 100" | bc -l)
idle_time=$(echo "scale=2; ${array[3]} / 100" | bc -l)
printf "    USER: %s seconds\n" "$user_time"
printf "    SYSTEM: %s seconds\n" "$sys_time"
printf "    IDLE: %s seconds\n\n" "$idle_time"

# Check how much memory is on the machine using free
echo "How much memory is on the machine?"
free | grep Mem | cut -c 5-19 | awk '{gsub(/ /, "", $0); printf"    MemTotal: %14i kB\n\n", $0'}

# Check how much memory is available on the machine using free
echo "How much memory is currently available?"
free | grep Mem | cut -c 68- | awk '{gsub(/ /, "", $0); printf"    MemFree: %15i kB\n\n", $0'}

# See how much has been read and written to disk using iostat
echo "How many kBytes have been read and written to the disk since the last reboot?"
str=$(iostat | grep -A 1 Device | tail -n 1 | cut -c 5-)
array=(${str//:/ })
printf "    Read: %s kB\n" "${array[3]}"
printf "    Written: %s kB\n\n" "${array[4]}"

# See how many processes have been created since last reboot using /proc/stat
echo "How many processes have been created since the last reboot?"
echo -e "    Processes created: "$(less /proc/stat | grep processes | cut -c 11-)"\n"

# See how many context switches have been made since last reboot using /proc/stat 
echo "How many context switches have been performed since the last reboot?"
echo -e "    Context switches: "$(less /proc/stat | grep ctxt | cut -c 6-)"\n"

# Find load averages for 1, 5 and 15 minutes respectively using uptime
echo "What is the current load average for the last 1, 5 and 15 minutes?"
echo -e "    "$(uptime | sed 's/^.*load/load/' | sed -E 's/(^[a-z]|\. [a-z])/\U&\E/g')"\n"
