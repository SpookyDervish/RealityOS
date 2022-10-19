# RealityOS
One day I decided I wanted to make an OS. And so I did. While it took me over a month to stop hitting a brick wall of trying to get interrupts working, I finally had it. It was smooth sailing from there. But overall, this was just a side project because I'm not really gonna work on this anymore.

# NOTICE
If you decide to compile this spaghetti monster of code, you will have lots of warnings. **Ignore them :)**
Oh yeah also this has a lot of bugs and I'm too lazy to fix them. :P

# Building
While **Docker** was used in the production of RealityOS, you may something else if you prefer, although, I will only be showing how to setup with Docker.

**TIP**: Make sure you have Docker running.
**TIP**: Make sure you are in RealityOS's root folder.

I have created some `.bat` files to make everyones' life easier.
Run the **docker-build.bat**, this will create the Docker image.

After it finishes building, run the **docker-run.bat** file, once in the terminal, run make build-x86_64.
Once it finishes building, you can use **QEMU** bu running **run.bat** or use VirtualBox with the new ISO in the dist folder.
