# Alien Flying Saucer Animation Generator

This tool generates a Full-HD (1920x1080) animation featuring an alien in a flying saucer with comedic burp and fart effects.

## Description

The animation tells the story of:
1. An alien sitting in his flying saucer, drinking 3 beer bottles in parallel
2. A human with Rastafari hair walks by eating a big cheeseburger
3. The flying saucer navigates (appearing invisible to the human) directly in front of his head
4. The alien burps so powerfully that the man's cheeseburger flies 30 meters away
5. The alien then farts, causing the man's Rastafari hair to fly around like a tornado

## Features

- **Full-HD Resolution**: 1920x1080 pixels
- **30 FPS**: Smooth animation
- **15 seconds duration**: Complete story arc
- **Procedural Graphics**: All elements drawn programmatically using PIL
- **MP4 Output**: Compatible with all video players

## Requirements

### System Requirements
- Python 3.6 or higher
- pip package manager

### Python Dependencies
```bash
pip install moviepy pillow numpy
```

### Additional System Dependencies
MoviePy requires ffmpeg for video encoding:

**On Ubuntu/Debian:**
```bash
sudo apt-get install ffmpeg
```

**On macOS:**
```bash
brew install ffmpeg
```

**On Windows:**
Download ffmpeg from https://ffmpeg.org/download.html and add to PATH

## Usage

### Generate the Animation

```bash
python3 generate_alien_animation.py
```

This will create `alien_animation.mp4` in the current directory.

### Customize Parameters

You can modify the following parameters in the script:

```python
WIDTH = 1920        # Video width in pixels
HEIGHT = 1080       # Video height in pixels
FPS = 30           # Frames per second
DURATION = 15      # Total duration in seconds

# Scene timing (in seconds)
SCENE_1_DURATION = 3.0  # Alien drinking
SCENE_2_DURATION = 2.0  # Human walking
SCENE_3_DURATION = 1.0  # Saucer moves in front
SCENE_4_DURATION = 4.0  # Burp effect
SCENE_5_DURATION = 5.0  # Fart effect
```

## Animation Breakdown

### Scene 1 (0-3 seconds)
- Alien sits in flying saucer in upper left
- Holding and drinking from 3 beer bottles simultaneously
- Saucer hovers peacefully

### Scene 2 (3-5 seconds)
- Human with Rastafari hair (red, yellow, green dreads) walks in from right
- Carrying a large cheeseburger
- Alien continues drinking

### Scene 3 (5-6 seconds)
- Flying saucer moves to position directly in front of human's head
- Movement is quick and stealthy
- Alien prepares for burp

### Scene 4 (6-10 seconds)
- Alien releases massive burp
- Green expanding wave effect
- Cheeseburger flies 30 meters (~900 pixels) away
- Burger follows parabolic trajectory

### Scene 5 (10-15 seconds)
- Alien releases fart
- Greenish cloud effect
- Rastafari hair spins wildly in tornado pattern
- Two full rotations with dreads flying in all directions

## Technical Details

### Drawing System
- Uses PIL (Python Imaging Library) for 2D graphics
- Each frame is rendered independently
- Supports alpha transparency for effects

### Animation Elements
1. **Flying Saucer**
   - Metallic gray ellipse body
   - Transparent blue dome
   - Yellow lights underneath
   
2. **Alien**
   - Classic green alien appearance
   - Large black eyes
   - Small body with articulated arms
   - Can hold multiple beer bottles
   
3. **Beer Bottles**
   - Brown glass bottles with caps
   - Golden beer visible inside
   - 3 bottles arranged for parallel drinking

4. **Human**
   - Natural skin tone
   - Rastafari dreads (red, yellow, green)
   - Red shirt, blue pants
   - Animated hair for tornado effect

5. **Cheeseburger**
   - Top bun with sesame seeds
   - Yellow cheese slice
   - Brown meat patty
   - Green lettuce
   - Bottom bun

6. **Effects**
   - Burp: Expanding green circular waves
   - Fart: Wavy green clouds
   - Physics-based burger trajectory
   - Tornado hair rotation

### Video Output
- Codec: H.264 (libx264)
- Format: MP4
- Bitrate: Automatic (based on quality)
- Audio: None (silent animation)

## File Structure

```
alien-animation/
├── README.md                        # This file
└── generate_alien_animation.py      # Main animation generator
```

## Performance

- Generation time: ~2-5 minutes depending on system
- File size: ~5-10 MB for 15-second video
- Memory usage: ~500 MB during generation

## Based On

This animation generator follows the pattern established by other tools in the C64 demos repository:
- `tools/mandelbrot-zoom/generate_mandelbrot_zoom.py` - Inspiration for frame-based animation
- `test/floodlights/generate_floodlights.py` - Pattern for procedural graphics

## License

Same as parent repository (GPL-3.0)
