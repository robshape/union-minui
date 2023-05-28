#!/bin/sh

incrementValueInFile() {
    local filename="$1"
    local current_value new_value

    # Check if the file exists
    if [ -f "$filename" ]; then
        # Read the current value from the file
        current_value=$(cat "$filename")

        # Increment the value by 30 seconds
        new_value=$((current_value + 30))

        # Write the new value back to the file
        echo "$new_value" > "$filename"
    fi
}

# File paths
charger_file="/sys/class/power_supply/battery/charger_online"
capacity_file="/sys/class/power_supply/battery/capacity"
stats_file="/mnt/sdcard/timemon.txt"

touch "$stats_file"
while true; do
    # Read the content of charger_online.txt and capacity.txt
    charger_status=$(cat "$charger_file")
    capacity=$(cat "$capacity_file")

    if [ "$charger_status" -eq 1 ] && [ "$capacity" -eq 300 ]; then
        # Reset the value of timemon.txt to 0 seconds
        echo 0 > "$stats_file"
    elif [ "$charger_status" -eq 0 ]; then
        # Increment the value of timemon.txt by 30 seconds
        incrementValueInFile "$stats_file"
    fi

    sleep 30
done
