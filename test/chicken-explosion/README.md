# Chicken Explosion Animation

A Full HD (1920x1080) comic-style animation of a chicken with explosives around its neck.

## Description

This animation shows a comic-style chicken going through the following sequence:

1. **Frames 0-29**: Chicken stands looking at the camera
2. **Frames 30-59**: Chicken picks/pecks on the floor
3. **Frames 60-89**: Chicken looks at the camera again
4. **Frames 90-119**: Chicken picks on the floor again
5. **Frames 120-179**: Chicken pecks at the camera glass multiple times
6. **Frames 180-209**: The explosive fuse burns down
7. **Frames 210-239**: The explosives detonate with a dramatic explosion

## Sample Frames

The repository includes sample frames showing key moments:
- `sample_frame_looking.png` - Chicken looking at camera
- `sample_frame_pecking.png` - Chicken pecking on floor
- `sample_frame_camera_peck.png` - Chicken pecking at camera glass
- `sample_frame_fuse.png` - Fuse burning
- `sample_frame_explosion.png` - Explosion effect

## Features

- Full HD resolution (1920x1080)
- Comic-style artwork with bold outlines
- Smooth animation (240 frames total, ~8 seconds at 30fps)
- TNT explosives necklace with burning fuse
- Dramatic explosion effect at the end

## Usage

### Generate Animation Frames

```bash
python3 generate_chicken_animation.py
```

This will create a `frames/` directory with 240 PNG images (frame_0000.png through frame_0239.png).

### Create Video (Optional)

If you have ffmpeg installed, you can combine the frames into a video:

```bash
ffmpeg -framerate 30 -i frames/frame_%04d.png -c:v libx264 -pix_fmt yuv420p chicken_explosion.mp4
```

## Requirements

- Python 3.x
- Pillow (PIL) library

Install requirements:
```bash
pip install pillow
```

## Animation Timeline

- **0-1 seconds**: Looking at camera (idle)
- **1-2 seconds**: Pecking on ground
- **2-3 seconds**: Looking at camera
- **3-4 seconds**: Pecking on ground again
- **4-6 seconds**: Pecking at camera glass
- **6-7 seconds**: Fuse burning
- **7-8 seconds**: Explosion!

## Technical Details

Each frame is rendered using PIL's ImageDraw module with:
- Custom chicken drawing function with adjustable head tilt, beak position, and eye direction
- TNT explosive necklace with multiple sticks
- Animated fuse with burning spark effect
- Multi-layered explosion with expanding circles and particles
