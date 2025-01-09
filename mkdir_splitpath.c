#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

// make directory
void mkdir(char pathName[]) {
    if (strcmp(pathName, "/") == 0) {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    char baseName[64];
    char dirName[64];

    struct NODE* parentPtr = splitPath(pathName, baseName, dirName);

    if (parentPtr == NULL) {
        // Error message already printed by splitPath()
        return;
    }

    // Check if baseName already exists in parentPtr's child list
    struct NODE* child = parentPtr->childPtr;
    while (child != NULL) {
        if (strcmp(child->name, baseName) == 0 && child->fileType == 'D') {
            // Directory already exists
            printf("MKDIR ERROR: directory %s already exists\n", pathName);
            return;
        }
        child = child->siblingPtr;
    }

    // Create new node
    struct NODE* newNode = (struct NODE*)malloc(sizeof(struct NODE));
    strcpy(newNode->name, baseName);
    newNode->fileType = 'D';
    newNode->parentPtr = parentPtr;
    newNode->childPtr = NULL;
    newNode->siblingPtr = NULL;

    // Add newNode as the last sibling of parentPtr's child list
    if (parentPtr->childPtr == NULL) {
        // First child
        parentPtr->childPtr = newNode;
    } else {
        // Find the last sibling
        struct NODE* lastSibling = parentPtr->childPtr;
        while (lastSibling->siblingPtr != NULL) {
            lastSibling = lastSibling->siblingPtr;
        }
        lastSibling->siblingPtr = newNode;
    }

    // Print success message
    printf("MKDIR SUCCESS: node %s successfully created\n", pathName);

    return;
}

// handles tokenizing and absolute/relative pathing options
struct NODE* splitPath(char* pathName, char* baseName, char* dirName) {
    // Handle the special case when pathName is "/"
    if (strcmp(pathName, "/") == 0) {
        strcpy(dirName, "/");
        strcpy(baseName, "");
    } else {
        char* lastSlash = strrchr(pathName, '/');
        if (lastSlash == NULL) {
            // No '/', pathName is baseName, dirName is ""
            strcpy(dirName, "");
            strcpy(baseName, pathName);
        } else if (lastSlash == pathName) {
            // The only slash is at the beginning
            strcpy(dirName, "/");
            strcpy(baseName, pathName + 1);
        } else {
            // There is a '/' somewhere in the middle
            int dirLength = lastSlash - pathName;  // Number of characters before last '/'
            strncpy(dirName, pathName, dirLength);
            dirName[dirLength] = '\0';  // null terminate
            strcpy(baseName, lastSlash + 1);
        }
    }

    // Decide starting point
    struct NODE* currentPtr;
    char currentPath[256] = "";

    if (dirName[0] == '/') {
        // Absolute path
        currentPtr = root;
    } else {
        // Relative path
        currentPtr = cwd;
    }

    // Now, tokenize dirName and traverse
    // Handle the case when dirName is empty or "/"
    if (strcmp(dirName, "") == 0 || strcmp(dirName, "/") == 0) {
        // dirName is empty or "/"
        // currentPtr is already set
    } else {
        // Need to traverse dirName
        // Make a copy of dirName to tokenize
        char dirCopy[64];
        strcpy(dirCopy, dirName);

        // strtok modifies the string, so we need to be careful
        char* token;
        if (dirName[0] == '/') {
            // Absolute path, skip the leading '/'
            token = strtok(dirCopy + 1, "/");
            strcpy(currentPath, "");
        } else {
            token = strtok(dirCopy, "/");
            strcpy(currentPath, "");
        }

        while (token != NULL) {
            if (currentPath[0] == '\0') {
                // First token
                strcpy(currentPath, token);
            } else {
                // Append "/" and token to currentPath
                strcat(currentPath, "/");
                strcat(currentPath, token);
            }

            // For each token, find the corresponding child node in currentPtr
            struct NODE* child = currentPtr->childPtr;
            int found = 0;
            while (child != NULL) {
                if (strcmp(child->name, token) == 0 && child->fileType == 'D') {
                    // Found the directory
                    currentPtr = child;
                    found = 1;
                    break;
                }
                child = child->siblingPtr;
            }
            if (!found) {
                // Directory does not exist
                printf("ERROR: directory %s does not exist\n", currentPath);
                return NULL;
            }
            // Get next token
            token = strtok(NULL, "/");
        }
    }

    // After traversal, currentPtr is the directory node we want
    // Return currentPtr
    return currentPtr;
}
