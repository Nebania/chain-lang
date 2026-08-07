The GUI system for Nebula OS Chain-Lang is now available as of 3/7/2026.
This is a guide to begin the Xorg server and start a GUI .link file.

Display should already be set to :0 based on the /etc/profile file. If not, set it manually:

    export DISPLAY=:0

Next, begin the Xorg server with the command:

    Xorg :0 &

YOUR SCREEN WILL GO BLACK. Simply press ctrl+alt+f1 to go back to the working tab.
You will see errors. These are expected. End Xorg with ctrl+c

With that, you are done! Link will handle the rest.
YOU MUST RUN LINK INSIDE THE GUI DIRECTORY. It looks for wallpaper.png and logo.png for gui_desktop.link

Nebula OS owes many thanks to those of the Xorg, Mesa, and Raylib community for their open source generosity.
Without them, Link would be a much more difficult task.

Thank you for using Nebula OS!
