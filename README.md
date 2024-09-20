# image_categorizer
A simple terminal application that allows categorization of image files

For my machine learning studies I want to categorize my image library. This is a simple terminal application that allows me to establish categories and then categorize images into those categories for model training purposes.

The program takes two arguments:

- The path to the directory containing the images
- The path in which directories will be created to store the categorized images

This program gets a list of all the images in the tree, presents the number of images found, and then asks for for the number of images to be categorized. 
It randomly selects images from the list and prompts for a list of categories.

The program then creates directories for each category and creates symlinks to the images to the respective directories.

I am going to use this in conjunction with the Coursera Guided Project [Basic Image Classification with TensorFlow](https://www.coursera.org/projects/tensorflow-beginner-basic-image-classification) to categorize images for training a model.

Most of this code was written using [ChatGPT](https://chatgpt.com/share/66edea05-4230-8007-bddb-c1c3e124837e).