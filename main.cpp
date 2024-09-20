#include <iostream>
    #include <filesystem>
#include <vector>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdlib>
#include <random>
#include <algorithm> // Include this for std::shuffle

namespace fs = std::filesystem;

pid_t showImage(const fs::path& imagePath) {
    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "Fork failed!" << std::endl;
        return -1;
    } else if (pid == 0) {
        // In child process
        setsid();
        execlp("feh", "feh", "--no-fehbg", imagePath.c_str(), nullptr);
        std::cerr << "execlp failed!" << std::endl;
        exit(1); // Exit if execlp fails
    }

    // Return the PID of the child process
    return pid;
}

void processImage(const fs::path& imagePath, const fs::path& targetDir, pid_t& pid) {
    std::vector<std::string> categories;
    std::string category;

    std::cout << "Processing: " << imagePath.filename() << std::endl;

    // Allow adding categories
    while (true) {
        std::cout << "Enter category (or type 'done' to finish): ";
        std::getline(std::cin, category);
        if (category == "done") {
            // Close the feh viewer by killing the process
            kill(pid, SIGTERM);
            waitpid(pid, nullptr, 0); // Wait for the feh process to terminate
            break;
        }
        if (!category.empty()) {
            categories.push_back(category); // Add only if not empty
            std::cout << "Category added: " << category << std::endl; // Echo category
        }
    }

    // Create directories and symlinks
    for (const auto& cat : categories) {
        fs::path catDir = targetDir / cat;
        fs::create_directory(catDir);
	fs::path symlinkPath = catDir / imagePath.filename();
	if (!fs::exists(symlinkPath)) {
          fs::create_symlink(imagePath, symlinkPath);
	} else {
	  std::cout << "Symlink already exists: " << symlinkPath << ". Skipping." << std::endl;
        }
    }
}

// Function to check if the file extension is in the list of allowed extensions
bool isImageExtension(const std::string& extension, const std::vector<std::string>& allowedExtensions) {
    return std::find(allowedExtensions.begin(), allowedExtensions.end(), extension) != allowedExtensions.end();
}

int main(int argc, char* argv[]) {
    std::vector<std::string> allowedExtensions = {".jpg", ".jpeg", ".png", ".bmp", ".gif"};

    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <source_directory> <target_directory>" << std::endl;
        return 1;
    }

    fs::path sourceDir(argv[1]);
    fs::path targetDir(argv[2]);

    if (!fs::is_directory(sourceDir)) {
        std::cerr << "Invalid source directory." << std::endl;
        return 1;
    }
    fs::create_directory(targetDir); // Ensure target directory exists

    std::vector<fs::path> images;
    for (const auto& entry : fs::recursive_directory_iterator(sourceDir)) {
        if (entry.is_regular_file() && isImageExtension(entry.path().extension(), allowedExtensions)) {
            images.push_back(entry.path());
        }
    }

    size_t imageCount = images.size();
    std::cout << "Total images found: " << imageCount << std::endl;

    if (imageCount == 0) {
        std::cout << "No images found in the source directory." << std::endl;
        return 0;
    }

    size_t sampleSize;
    std::cout << "Enter sample size (max " << imageCount << "): ";
    std::cin >> sampleSize;
    std::cin.ignore(); // Clear the newline character from the input buffer

    if (sampleSize > imageCount) {
        sampleSize = imageCount; // Adjust to max available if input exceeds count
    }

    // Shuffle and select random images
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(images.begin(), images.end(), g);
    images.resize(sampleSize); // Keep only the sample size

    for (const auto& image : images) {
        pid_t pid = showImage(image);
        processImage(image, targetDir, pid);
    }

    return 0;
}

