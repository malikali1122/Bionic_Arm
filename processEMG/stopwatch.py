from datetime import datetime
import time

start_time = 0


def start():
    global start_time
    start_time = time.time()

# Returns the time elapsed in us since start() was called
def get_elapsed_time():
    return round((time.time() - start_time)*1000000)
