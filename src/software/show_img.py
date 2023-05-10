import numpy as np
import matplotlib.pyplot as plt
import os

# Load the text file containing the pixel values
with open(os.path.join(os.path.dirname(os.path.abspath(__file__)),'rgb888img'), 'r') as f:
    pixel_values = np.loadtxt(f)

# Reshape the pixel values to create an image
print(pixel_values.shape)
height, width = pixel_values.shape[:2]
height, width = 480, 640
print(height, width)
image = pixel_values.reshape((height, width, 3)).astype(np.uint8)

# Display the image
plt.imshow(image)
plt.show()