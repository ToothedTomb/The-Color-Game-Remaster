#!/bin/bash

# Variables
APP_NAME="The Color Game Remaster"
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"  # Get the directory of this script
EXECUTABLE_PATH="${SCRIPT_DIR}/The Color Game Remaster"  # Path to the executable in the same directory
DESKTOP_FILE_DIR="$HOME/.local/share/applications"  # User-specific application menu
DESKTOP_FILE_PATH="$DESKTOP_FILE_DIR/color-game.desktop"  # Full path to the .desktop file

# Debugging information
echo "Script directory (SCRIPT_DIR): $SCRIPT_DIR"
echo "Executable path (EXECUTABLE_PATH): $EXECUTABLE_PATH"
echo "Desktop file path (DESKTOP_FILE_PATH): $DESKTOP_FILE_PATH"

# Ensure the .desktop directory exists
echo "Creating necessary directories..."
mkdir -p "$DESKTOP_FILE_DIR"

# Check if the executable exists
if [[ ! -f "$EXECUTABLE_PATH" ]]; then
   echo "Error: Executable not found at $EXECUTABLE_PATH. Please ensure it's in the same directory as this script."
   exit 1
fi

# Create the .desktop file content
DESKTOP_FILE_CONTENT="[Desktop Entry]
Name=$APP_NAME
Exec=\"$EXECUTABLE_PATH\"
Type=Application
Categories=Game;
Terminal=false"

# Create the .desktop file in the script directory first
echo "Creating the .desktop file in the current directory ($SCRIPT_DIR)..."
echo "$DESKTOP_FILE_CONTENT" > "$SCRIPT_DIR/color-game.desktop"

# Verify that the file was created
if [[ ! -f "$SCRIPT_DIR/color-game.desktop" ]]; then
   echo "Error: Failed to create the .desktop file in $SCRIPT_DIR."
   exit 1
fi

# Move the .desktop file to the user's applications directory
echo "Moving the .desktop file to $DESKTOP_FILE_PATH..."
mv "$SCRIPT_DIR/color-game.desktop" "$DESKTOP_FILE_PATH"

# Verify that the move was successful
if [[ ! -f "$DESKTOP_FILE_PATH" ]]; then
   echo "Error: Failed to move the .desktop file to $DESKTOP_FILE_PATH."
   exit 1
fi

# Set appropriate permissions for the .desktop file
chmod 644 "$DESKTOP_FILE_PATH"

# Refresh the application menu
echo "Refreshing the application menu..."
update-desktop-database "$DESKTOP_FILE_DIR" &>/dev/null || true

echo "$APP_NAME has been successfully added to your application menu!"
echo "You can now find it in your app launcher."

