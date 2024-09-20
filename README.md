# image_categorizer
A simple terminal application that allows categorization of image files

For my machine learning studies I want to categorize a lot of images. I have a lot of images in a folder and I want to categorize them into different folders. This is a simple terminal application that allows me to do that.

The program takes two arguments:

- The path to the directory containing the images
- The path in which directories will be created to store the categorized images


This program gets a list of all the images in the tree, presents the number of images found, and then asks for for the number of images to be categorized. 
It randomly selects images from the list and prompts for a list of categories.

The program then creates directories for each category and creates symlinks to the images to the respective directories.

I am going to use this in conjunction with the Coursera Guided Project [Basic Image Classification with TensorFlow](https://www.coursera.org/projects/tensorflow-beginner-basic-image-classification) to categorize images for training a model.

