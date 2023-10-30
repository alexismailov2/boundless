#!/bin/bash

# Update package list and install prerequisites
sudo apt-get update
sudo apt-get install -y apt-transport-https ca-certificates curl software-properties-common

# Add Docker's official GPG key
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -

# Verify fingerprint
FINGERPRINT_MATCH=$(sudo apt-key fingerprint 0EBFCD88 | grep '9DC8 5822 9FC7 DD38 854A  E2D8 8D81 803C 0EBF CD88')
if [ -z "$FINGERPRINT_MATCH" ]; then
    echo "Docker GPG key fingerprint verification failed!"
    exit 1
fi

# Set up the Docker stable repository
sudo add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable"

# Update package database with Docker packages
sudo apt-get update

# Ensure the repository is set up correctly
REPO_CHECK=$(apt-cache policy docker-ce | grep 'download.docker.com')
if [ -z "$REPO_CHECK" ]; then
    echo "Docker repository setup failed!"
    exit 1
fi

# Install Docker
sudo apt-get install -y docker-ce

# Add user to the docker group (optional)
sudo usermod -aG docker ${USER}

echo "Docker installation completed. Please log out and log back in for group changes to take effect."
