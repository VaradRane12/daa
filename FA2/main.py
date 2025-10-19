from ursina import *

# --- 1. Maze Setup ---
# 0 = Path, 1 = Wall, S = Start, E = End
maze_layout = [
    [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
    ['S',0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1],
    [1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1],
    [1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1],
    [1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1],
    [1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 'E', 1],
    [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
]

# --- 2. Backtracking Algorithm ---
# A generator function that yields each step of the algorithm
def solve_maze(maze, position):
    x, z = position
    
    # Base Case: Check if we are out of bounds or hit a wall
    if not (0 <= x < len(maze) and 0 <= z < len(maze[0]) and maze[x][z] != 1):
        return
        
    # Mark as part of the current path and yield the step
    path.add((x, z))
    yield (x, z, 'visiting')

    # Base Case: If we've reached the end
    if maze[x][z] == 'E':
        # Mark the entire correct path as the solution
        for pos in path:
            yield (pos[0], pos[1], 'solution')
        # This special yield signals the end of the simulation
        yield (-1, -1, 'done')
        return

    # Recursive Step: Try to move in four directions (Down, Right, Up, Left)
    moves = [(x + 1, z), (x, z + 1), (x - 1, z), (x, z - 1)]
    for next_x, next_z in moves:
        if (next_x, next_z) not in path: # Don't go back to a tile we are already on
            # The 'yield from' passes control to the recursive call
            # This is key to making the whole recursive process step-by-step
            yield from solve_maze(maze, (next_x, next_z))
            # If the simulation is done, stop exploring
            if is_done:
                return
    
    # If we are here, it means no moves from (x,z) led to the solution. Backtrack.
    path.remove((x, z))
    yield (x, z, 'backtracking')


# --- 3. Ursina Application ---
app = Ursina()

# Create a dictionary to hold our floor tile entities for easy access
floor_tiles = {}
start_pos = (0, 0)

# Generate the 3D maze from the layout
for r, row in enumerate(maze_layout):
    for c, item in enumerate(row):
        # The maze is read row-by-row (like x,y) but we place it in 3D space (x,z)
        x_pos, z_pos = r, c
        
        if item == 1:
            Entity(model='cube', color=color.gray, position=(x_pos, 0, z_pos), texture='white_cube')
        else:
            tile = Entity(model='plane', position=(x_pos, -0.5, z_pos), texture='white_cube', scale=1)
            floor_tiles[(x_pos, z_pos)] = tile # Store the tile entity
            if item == 'S':
                tile.color = color.green
                start_pos = (x_pos, z_pos)
            elif item == 'E':
                tile.color = color.gold
            else:
                tile.color = color.light_gray

# Global variables to control the simulation
path = set() # Stores the current path of the algorithm
solver = solve_maze(maze_layout, start_pos) # Create the generator
is_done = False
visualization_speed = 0.05 # Time in seconds between steps
time_accumulator = 0

# The update function is called every frame by Ursina
def update():
    global time_accumulator, is_done
    
    if is_done:
        return
        
    time_accumulator += time.dt
    
    # Only proceed if enough time has passed
    if time_accumulator >= visualization_speed:
        time_accumulator = 0
        try:
            x, z, status = next(solver)
            
            if status == 'done':
                print("Solution Found!")
                is_done = True
                return

            tile = floor_tiles.get((x, z))
            if tile:
                if status == 'visiting':
                    tile.color = color.azure
                elif status == 'backtracking':
                    tile.color = color.dark_gray
                elif status == 'solution':
                    tile.color = color.lime
                    
        except StopIteration:
            print("No solution found.")
            is_done = True

# Setup the camera to view the maze from above
EditorCamera(rotation_x=90, y=15, z=-10, rotation_z=90)

app.run()