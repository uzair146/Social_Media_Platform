#include <iostream>
#include <string>
#include <windows.h>
#include <fstream>
#include <sstream>
#include <ctime>
using namespace std;
// FORWARD DECLARATION OF STRUCTS
struct PostNode;
struct UserNode;
struct StoryNode;
struct NotificationNode;
struct MessageNode;
struct AVLNode;
struct FriendNode;
struct Conversation;
// FORWARD DECLARATION OF FUNCTIONS
int getInt();
int hashFunction(string userName);
bool checkUserExists(string userName);
string getCurrentTimestamp();
void updateLastActive(UserNode* user);
void initializeHashTable();
void registerUser();
void loginUser();
void logoutUser();
void searchUser();
void displayAllUsers();
void addFriendEdge(UserNode* user, string friendName);
void removeFriendEdge(UserNode* user, string friendName);
void addFriend();
void removeFriend();
void displayFriends();
void BFS(string startUser);
void DFSHelper(string current, string target, string visited[], int& visitedCount, bool& found);
void DFS(string startUser, string targetUser);
void addToFeedList(PostNode* post);
void removeFromFeedList(string postID);
PostNode* findPost(string postID, string& owner);
void createPost();
void deletePost();
void displayMyPosts();
void likePost();
void navigateFeed();
void addStory();
void viewStories();
void deleteUserStories(string userName);
void enqueueNotification(string message, string userName);
void dequeueNotification();
void peekNotification();
void displayNotifications();
void sendLikeNotification(string postOwner, string likerName);
void sendMessageNotification(string receiverName, string senderName);
Conversation* findConversation(string userA, string userB);
Conversation* getOrCreateConversation(string userA, string userB);
void sendMessage();
void viewLatestMessage();
void popMessage();
void displayConversation();
void deleteUserMessages(string userName);
int getHeight(AVLNode* n);
int getBalance(AVLNode* n);
AVLNode* rightRotate(AVLNode* y);
AVLNode* leftRotate(AVLNode* x);
AVLNode* insertAVL(AVLNode* node, string key, int value);
AVLNode* deleteAVL(AVLNode* root, string key);
AVLNode* searchAVL(AVLNode* node, string key);
void updateAVLValue(AVLNode*& root, string key, int newVal);
AVLNode* findMinNode(AVLNode* node);
void updateUserActivityInAVL(string userName);
void updatePostLikesInAVL(string postID, int newLikes);
void addPostToAVL(string postID);
void removePostFromAVL(string postID);
void removeUserFromAVL(string userName);
void getTopKHelper(AVLNode* root, int& count, int k, string title, bool& printed);
void getTopK(AVLNode* root, int k, string title);
void inorderPrint(AVLNode* root);
void inorderDisplay(AVLNode* root, string title);
void rangeQueryHelper(AVLNode* root, int minVal, int maxVal, bool& found);
void rangeQuery(AVLNode* root, int minVal, int maxVal, string title);
void deleteAllUserPosts(string userName);
void removeUserFromGraph(string userName);
void completeUserDeletion(string userName);
void deleteUser();
void displayUserMenu();
void userManagementModule();
void displaySocialNetworkMenu();
void socialNetworkModule();
void displayPostsMenu();
void postsModule();
void displayStoriesMenu();
void storiesModule();
void displayNotificationsMenu();
void notificationsModule();
void displayMessagingMenu();
void messagingModule();
void displayAnalyticsMenu();
void analyticsModule();
void displayMainMenu();
void saveUsersToFile();
void loadUsersFromFile();
void saveFriendsToFile();
void loadFriendsFromFile();
void savePostsToFile();
void loadPostsFromFile();
void saveStoriesToFile();
void loadStoriesFromFile();
void saveMessagesToFile();
void loadMessagesFromFile();
void saveNotificationsToFile();
void loadNotificationsFromFile();
void saveAllData();
void loadAllData();
// FUNCTION FOR INVALID CHOICE INPUT
int getInt() {
    int val;
    while (true) {
        cin >> val;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << endl;
            cout << "\033[31mInvalid Input!\033[0m" << endl;
            Beep(800, 300);
            cout << "\033[34mEnter a number: \033[0m";
        }
        else {
            cin.ignore(1000, '\n');
            return val;
        }
    }
}
// USER NODE STRUCT
struct UserNode {
    string userName;
    string password;
    string email;
    string bio;
    string lastActive;
    UserNode* next;
    PostNode* postsHead;
    FriendNode* friendsHead;
    UserNode(string name, string pwd, string mail, string b) {
        userName = name;
        password = pwd;
        email = mail;
        bio = b;
        lastActive = getCurrentTimestamp();
        next = nullptr;
        postsHead = nullptr;
        friendsHead = nullptr;
    }
};
// POST NODE STRUCT
struct PostNode {
    string postID;
    string content;
    string owner;
    int likes;
    PostNode* next;
    PostNode* prev;
    PostNode(string id, string cont, string own) {
        postID = id;
        content = cont;
        owner = own;
        likes = 0;
        next = nullptr;
        prev = nullptr;
    }
};
// STORY NODE STRUCT
struct StoryNode {
    string userName;
    string storyContent;
    StoryNode* next;
    StoryNode(string name, string content) {
        userName = name;
        storyContent = content;
        next = nullptr;
    }
};
// NOTIFICATION NODE STRUCT
struct NotificationNode {
    string message;
    string userName;
    string timestamp;
    NotificationNode* next;
    NotificationNode(string msg, string name, string time) {
        message = msg;
        userName = name;
        timestamp = time;
        next = nullptr;
    }
};
// MESSAGE NODE STRUCT
struct MessageNode {
    string fromUser;
    string toUser;
    string text;
    string timestamp;
    MessageNode* next;
    MessageNode(string from, string to, string txt, string time) {
        fromUser = from;
        toUser = to;
        text = txt;
        timestamp = time;
        next = nullptr;
    }
};
// AVL NODE STRUCT
struct AVLNode {
    string key;
    int value;
    AVLNode* left;
    AVLNode* right;
    int height;
    AVLNode(string k, int val) {
        key = k;
        value = val;
        left = nullptr;
        right = nullptr;
        height = 1;
    }
};
// FRIEND NODE STRUCT
struct FriendNode {
    string friendUserName;
    FriendNode* next;
    FriendNode(string name) {
        friendUserName = name;
        next = nullptr;
    }
};
// CONVERSATION STRUCT
struct Conversation {
    string user1;
    string user2;
    MessageNode* stackTop;
    Conversation* next;
    Conversation() {
        user1 = "";
        user2 = "";
        stackTop = nullptr;
        next = nullptr;
    }
};
// GLOBAL VARIABLES,POINTERS
const int TABLE_SIZE = 100;
UserNode* hashTable[TABLE_SIZE];
UserNode* currentUser = nullptr;
StoryNode* storiesHead = nullptr;
NotificationNode* notificationFront = nullptr;
NotificationNode* notificationRear = nullptr;
int postCounter = 1;
Conversation* conversationsList = nullptr;
AVLNode* userAVLRoot = nullptr;
AVLNode* postAVLRoot = nullptr;
PostNode* feedHead = nullptr;
PostNode* feedCurrent = nullptr;
// INITIALIZE ALL HASH TABLE BUCKETS TO NULL
void initializeHashTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = nullptr;
    }
}
// HASH FUNCTION TO MAP USERNAME TO HASH TABLE INDEX
int hashFunction(string userName) {
    int sum = 0;
    for (int i = 0; i < (int)userName.length(); i++) {
        sum += (int)userName[i];
    }
    return sum % TABLE_SIZE;
}
// CHECK IF A USERNAME ALREADY EXISTS IN THE SYSTEM
bool checkUserExists(string userName) {
    int idx = hashFunction(userName);
    UserNode* temp = hashTable[idx];
    while (temp) {
        if (temp->userName == userName) {
            return true;
        }
        temp = temp->next;
    }
    return false;
}
// GET CURRENT TIMESTAMP STRING (REAL TIME)
string getCurrentTimestamp() {
    time_t now = time(0);
    char* dt = ctime(&now);
    string timestamp(dt);
    timestamp.pop_back(); 
    return timestamp;
}
// UPDATE THE LAST ACTIVE TIME OF A USER
void updateLastActive(UserNode* user) {
    if (user) {
        user->lastActive = getCurrentTimestamp();
    }
}
// REGISTER A NEW USER ACCOUNT
void registerUser() {
    string un, pwd, em, bio;
    system("cls");
    cout << "\n\033[1;36m--- REGISTER ---\033[0m" << endl;
    cout << "\033[34mUsername: \033[0m";
    getline(cin, un);
    int idx = hashFunction(un);
    UserNode* t = hashTable[idx];
    while (t) {
        if (t->userName == un) {
            system("cls");
            Beep(900, 370);
            cout << "\033[31mUsername already exists.\033[0m" << endl;
            cout << "\033[34mChoose another.\033[0m" << endl;
            cout << endl;
            return;
        }
        t = t->next;
    }
    while (true) {
        cout << "\033[34mPassword\033[0m \033[31m(min 8 characters): \033[0m";
        getline(cin, pwd);
        if (pwd.length() >= 8) {
            break;
        }
        cout << "\033[31mPassword must be at least 8 characters.\033[0m" << endl;
        cout << endl;
    }
    while (true) {
        cout << "\033[34mEmail: \033[0m";
        getline(cin, em);
        bool atFound = false;
        bool dotAfterAt = false;
        int atPos = -1;
        for (int i = 0; i < (int)em.length(); i++) {
            if (em[i] == '@') {
                atFound = true;
                atPos = i;
                break;
            }
        }
        if (atFound) {
            for (int i = atPos + 1; i < (int)em.length(); i++) {
                if (em[i] == '.') {
                    dotAfterAt = true;
                    break;
                }
            }
        }
        if (atFound && dotAfterAt) {
            break;
        }
        cout << "\033[31mInvalid email.\033[0m" << endl;
        cout << "\033[34mMust contain\033[0m \033[33m'@'\033[0m \033[34m and a \033[0m \033[33mdot\033[0m \033[34mafter\033[0m \033[33m'@'\033[0m \033[034m(e.g.,\033[0m \033[32muser@gmail.com\033[0m \033[34m)\033[0m" << endl;
        cout << endl;
    }
    cout << "\033[34mBio: \033[0m";
    getline(cin, bio);
    UserNode* nu = new UserNode(un, pwd, em, bio);
    nu->next = hashTable[idx];
    hashTable[idx] = nu;
    userAVLRoot = insertAVL(userAVLRoot, un, 0);
    cout << endl;
    system("cls");
    Beep(850, 120);
    cout << "\033[1;32mRegistered! Please login.\033[0m" << endl;
    cout << endl;
    saveUsersToFile();
}
// LOG AN EXISTING USER INTO THE SYSTEM
void loginUser() {
    string un, pwd;
    system("cls");
    cout << "\n\033[1;36m--- LOGIN ---\033[0m" << endl;
    cout << "\033[34mUsername: \033[0m";
    getline(cin, un);
    cout << "\033[34mPassword: \033[0m";
    getline(cin, pwd);
    int idx = hashFunction(un);
    UserNode* t = hashTable[idx];
    while (t) {
        if (t->userName == un && t->password == pwd) {
            currentUser = t;
            updateLastActive(currentUser);
            cout << endl;
            system("cls");
            Beep(1000, 200);
            cout << "\033[1;32mWelcome " << un << ".\033[0m" << endl;
            cout << endl;
            return;
        }
        t = t->next;
    }
    cout << endl;
    system("cls");
    Beep(700, 400);
    cout << "\033[31mInvalid credentials.\033[0m" << endl;
    cout << endl;
}
// LOG OUT THE CURRENTLY LOGGED-IN USER
void logoutUser() {
    if (!currentUser) {
        cout << endl;
        system("cls");
        cout << "\033[31mNo user logged in.\033[0m" << endl;
        cout << endl;
        return;
    }
    cout << endl;
    system("cls");
    Beep(800, 200);
    cout << "\033[33mGoodbye " << currentUser->userName << ".\033[0m" << endl;
    cout << endl;
    currentUser = nullptr;
}
// SEARCH FOR A USER BY USERNAME AND DISPLAY THEIR INFO
void searchUser() {
    string un;
    system("cls");
    cout << "\033[34mUsername: \033[0m";
    cin >> un;
    cin.ignore(1000, '\n');
    int idx = hashFunction(un);
    UserNode* t = hashTable[idx];
    while (t) {
        if (t->userName == un) {
            cout << endl;
            system("cls");
            Beep(900, 150);
            cout << "\033[92mFound: " << t->userName << " | Email: " << t->email << " | Bio: " << t->bio << " | Active: " << t->lastActive << "\033[0m" << endl;
            cout << endl;
            return;
        }
        t = t->next;
    }
    cout << endl;
    system("cls");
    Beep(700, 300);
    cout << "\033[31mNot found.\033[0m" << endl;
    cout << endl;
}
// DISPLAY ALL REGISTERED USERS IN THE SYSTEM
void displayAllUsers() {
    system("cls");
    cout << "\n\033[1;36m--- ALL USERS ---\033[0m" << endl;
    bool any = false;
    for (int i = 0; i < TABLE_SIZE; i++) {
        UserNode* t = hashTable[i];
        while (t) {
            cout << "\033[32m=> " << t->userName << "\033[35m (" << t->email << ")\033[0m" << endl;
            any = true;
            t = t->next;
        }
    }
    if (!any) {
        system("cls");
        cout << "\033[33mNo users.\033[0m" << endl;
    }
    cout << endl;
}
// ADD A FRIEND EDGE FROM ONE USER TO ANOTHER
void addFriendEdge(UserNode* user, string friendName) {
    FriendNode* f = user->friendsHead;
    while (f) {
        if (f->friendUserName == friendName) {
            return;
        }
        f = f->next;
    }
    FriendNode* nf = new FriendNode(friendName);
    nf->next = user->friendsHead;
    user->friendsHead = nf;
}
// REMOVE A FRIEND EDGE FROM ONE USER TO ANOTHER 
void removeFriendEdge(UserNode* user, string friendName) {
    FriendNode* prev = nullptr;
    FriendNode* cur = user->friendsHead;
    while (cur) {
        if (cur->friendUserName == friendName) {
            if (prev) {
                prev->next = cur->next;
            }
            else {
                user->friendsHead = cur->next;
            }
            delete cur;
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}
// ADD A FRIEND RELATIONSHIP
void addFriend() {
    if (!currentUser) {
        system("cls");
        cout << "\033[31mLogin first.\033[0m" << endl;
        return;
    }
    string fn;
    cout << "\033[34mFriend username: \033[0m";
    cin >> fn;
    cin.ignore(1000, '\n');
    if (!checkUserExists(fn)) {
        system("cls");
        cout << "\033[31mNo such user.\033[0m" << endl;
        return;
    }
    if (fn == currentUser->userName) {
        system("cls");
        cout << "\033[31mCannot add self.\033[0m" << endl;
        return;
    }
    addFriendEdge(currentUser, fn);
    int idx = hashFunction(fn);
    UserNode* fu = hashTable[idx];
    while (fu && fu->userName != fn) {
        fu = fu->next;
    }
    if (fu) {
        addFriendEdge(fu, currentUser->userName);
    }
    Beep(950, 150);
    system("cls");
    cout << "\033[1;32mFriend added.\033[0m" << endl;
    saveFriendsToFile();
}
// REMOVE A FRIEND RELATIONSHIP 
void removeFriend() {
    if (!currentUser) {
        system("cls");
        cout << "\033[31mLogin first.\033[0m" << endl;
        return;
    }
    string fn;
    cout << "\033[34mFriend to remove: \033[0m";
    cin >> fn;
    cin.ignore(1000, '\n');
    removeFriendEdge(currentUser, fn);
    int idx = hashFunction(fn);
    UserNode* fu = hashTable[idx];
    while (fu && fu->userName != fn) fu = fu->next;
    if (fu) removeFriendEdge(fu, currentUser->userName);
    system("cls");
    Beep(800, 200);
    cout << "\033[1;32mRemoved.\033[0m" << endl;
    saveFriendsToFile();
}
// DISPLAY ALL FRIENDS OF THE CURRENT USER
void displayFriends() {
    if (!currentUser) {
        system("cls");
        cout << "\033[31mLogin first.\033[0m" << endl;
        return;
    }
    system("cls");
    cout << "\n\033[1;36m--- YOUR FRIENDS ---\033[0m" << endl;
    FriendNode* f = currentUser->friendsHead;
    if (!f) {
        system("cls");
        cout << "\033[33mNo friends.\033[0m" << endl;
        return;
    }
    int c = 0;
    system("cls");
    while (f) {
        cout << "\033[92m" << ++c << ". " << f->friendUserName << "\033[0m" << endl;
        f = f->next;
    }
    cout << "\033[93mTotal: " << c << "\033[0m" << endl;
}
// PERFORM BREADTH-FIRST SEARCH FROM A START USER
void BFS(string startUser) {
    if (!checkUserExists(startUser)) {
        system("cls");
        cout << "\033[31mUser not exist.\033[0m" << endl;
        return;
    }
    string q[1000];
    int front = 0, rear = 0;
    string visited[1000];
    int vc = 0;
    q[rear++] = startUser;
    system("cls");
    cout << "\033[1;36mBFS from " << startUser << ": \033[0m";
    while (front < rear) {
        string cur = q[front++];
        bool v = false;
        for (int i = 0; i < vc; i++) {
            if (visited[i] == cur) {
                v = true;
                break;
            }
        }
        if (!v) {
            cout << "\033[92m" << cur << " \033[0m";
            visited[vc++] = cur;
            int idx = hashFunction(cur);
            UserNode* u = hashTable[idx];
            while (u && u->userName != cur) {
                u = u->next;
            }
            if (u) {
                FriendNode* f = u->friendsHead;
                while (f) {
                    q[rear++] = f->friendUserName;
                    f = f->next;
                }
            }
        }
    }
    cout << endl;
}
// RECURSIVE HELPER FOR DEPTH-FIRST SEARCH
void DFSHelper(string cur, string target, string visited[], int& vc, bool& found) {
    for (int i = 0; i < vc; i++) {
        if (visited[i] == cur) {
            return;
        }
    }
    visited[vc++] = cur;
    system("cls");
    cout << "\033[92m" << cur << " \033[0m";
    if (cur == target) {
        found = true;
        return;
    }
    int idx = hashFunction(cur);
    UserNode* u = hashTable[idx];
    while (u && u->userName != cur) {
        u = u->next;
    }
    if (u) {
        FriendNode* f = u->friendsHead;
        while (f && !found) {
            DFSHelper(f->friendUserName, target, visited, vc, found);
            f = f->next;
        }
    }
}
// PERFORM DEPTH-FIRST SEARCH FROM START USER TO TARGET
void DFS(string startUser, string targetUser) {
    if (!checkUserExists(startUser) || !checkUserExists(targetUser)) {
        system("cls");
        cout << "\033[31mUser not exist.\033[0m" << endl;
        return;
    }
    string visited[1000];
    int vc = 0;
    bool found = false;
    system("cls");
    cout << "\033[1;36mDFS path from " << startUser << " to " << targetUser << ": \033[0m";
    DFSHelper(startUser, targetUser, visited, vc, found);
    cout << endl;
    if (found) {
        Beep(950, 150);
        cout << "\033[1;32mReachable.\033[0m" << endl;
    }
    else {
        Beep(700, 250);
        cout << "\033[33mNot reachable.\033[0m" << endl;
    }
}
// ADD A POST TO THE GLOBAL FEED
void addToFeedList(PostNode* post) {
    if (!feedHead) {
        feedHead = feedCurrent = post;
        post->prev = nullptr;
        post->next = nullptr;
        return;
    }
    post->next = feedHead;
    feedHead->prev = post;
    post->prev = nullptr;
    feedHead = post;
}
// REMOVE A POST FROM THE GLOBAL FEED BY ITS ID
void removeFromFeedList(string postID) {
    PostNode* cur = feedHead;
    while (cur) {
        if (cur->postID == postID) {
            if (cur->prev) {
                cur->prev->next = cur->next;
            }
            else {
                feedHead = cur->next;
            }
            if (cur->next) {
                cur->next->prev = cur->prev;
            }
            if (feedCurrent == cur) {
                if (cur->next) {
                    feedCurrent = cur->next;
                }
                else {
                    feedCurrent = cur->prev;
                }
            }
            return;
        }
        cur = cur->next;
    }
}
// FIND A POST BY ITS ID ACROSS ALL USERS, RETURNING OWNER NAME
PostNode* findPost(string postID, string& owner) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        UserNode* u = hashTable[i];
        while (u) {
            PostNode* p = u->postsHead;
            while (p) {
                if (p->postID == postID) {
                    owner = u->userName;
                    return p;
                }
                p = p->next;
            }
            u = u->next;
        }
    }
    return nullptr;
}
// CREATE A NEW POST FOR THE CURRENT USER
void createPost() {
    if (!currentUser) {
        system("cls");
        cout << "\033[31mLogin first.\033[0m" << endl;
        return;
    }
    string content;
    system("cls");
    cout << "\033[34mPost content: \033[0m";
    getline(cin, content);
    string pid = "POST" + to_string(postCounter++);
    PostNode* np = new PostNode(pid, content, currentUser->userName);
    np->next = currentUser->postsHead;
    if (currentUser->postsHead) currentUser->postsHead->prev = np;
    currentUser->postsHead = np;
    addToFeedList(np);
    addPostToAVL(pid);
    updateUserActivityInAVL(currentUser->userName);
    updateLastActive(currentUser);
    system("cls");
    Beep(1000, 200);
    cout << "\033[1;32mPost created. ID: " << pid << "\033[0m" << endl;
    savePostsToFile();
}
// DELETE A POST OWNED BY THE CURRENT USER
void deletePost() {
    if (!currentUser) {
        system("cls");
        cout << "\033[31mLogin first.\033[0m" << endl;
        return;
    }
    string pid;
    system("cls");
    cout << "\033[34mPost ID: \033[0m";
    cin >> pid;
    cin.ignore(1000, '\n');
    PostNode* cur = currentUser->postsHead;
    while (cur) {
        if (cur->postID == pid) {
            removePostFromAVL(pid);
            removeFromFeedList(pid);
            if (cur->prev) {
                cur->prev->next = cur->next;
            }
            else {
                currentUser->postsHead = cur->next;
            }
            if (cur->next) {
                cur->next->prev = cur->prev;
            }
            delete cur;
            Beep(850, 180);
            cout << "\033[1;32mDeleted.\033[0m" << endl;
            savePostsToFile();
            return;
        }
        cur = cur->next;
    }
    Beep(700, 250);
    cout << "\033[31mNot found or not yours.\033[0m" << endl;
}
// DISPLAY ALL POSTS OF THE CURRENT USER
void displayMyPosts() {
    if (!currentUser) {
        system("cls");
        cout << "\033[31mLogin first.\033[0m" << endl;
        return;
    }
    PostNode* p = currentUser->postsHead;
    if (!p) {
        system("cls");
        cout << "\033[33mNo posts.\033[0m" << endl;
        return;
    }
    int cnt = 0;
    system("cls");
    while (p) {
        cout << "\033[93m[" << ++cnt << "] \033[96m" << p->postID << "\033[0m : " << p->content << " \033[35m(Likes: " << p->likes << ")\033[0m" << endl;
        p = p->next;
    }
}
// LIKE A POST
void likePost() {
    if (!currentUser) {
        system("cls");
        cout << "\033[31mLogin first.\033[0m" << endl;
        return;
    }
    string pid;
    system("cls");
    cout << "\033[34mPost ID: \033[0m";
    cin >> pid;
    cin.ignore(1000, '\n');
    string owner;
    PostNode* p = findPost(pid, owner);
    if (!p) {
        cout << "\033[31mPost not found.\033[0m" << endl;
        return;
    }
    if (owner == currentUser->userName) {
        cout << "\033[33mCannot like own post.\033[0m" << endl;
        return;
    }
    p->likes++;
    updatePostLikesInAVL(pid, p->likes);
    sendLikeNotification(owner, currentUser->userName);
    Beep(1000, 150);
    cout << "\033[1;32mLiked! Now " << p->likes << " likes.\033[0m" << endl;
    savePostsToFile();
}
// INTERACTIVE FEED NAVIGATION
void navigateFeed() {
    if (!currentUser) {
        system("cls");
        cout << "\033[31mLogin first.\033[0m" << endl;
        return;
    }
    if (!feedHead) {
        system("cls");
        cout << "\033[33mNo posts.\033[0m" << endl;
        return;
    }
    if (!feedCurrent) {
        feedCurrent = feedHead;
    }
    char ch;
    do {
        system("cls");
        cout << "\n\033[1;36m===== FEED =====\033[0m" << endl;
        cout << "\033[93mBy: " << feedCurrent->owner << "\033[0m" << endl;
        cout << "\033[97m" << feedCurrent->content << "\033[0m" << endl;
        cout << "\033[35mLikes: " << feedCurrent->likes << "\033[0m" << endl;
        cout << "\033[33m[N]ext [P]rev [L]ike [B]ack: \033[0m";
        cin >> ch;
        cin.ignore(1000, '\n');
        switch (ch) {
        case 'N': case 'n': {
            if (feedCurrent->next) {
                feedCurrent = feedCurrent->next;
                Beep(600, 100);
            }
            else {
                cout << "\033[33mLast post.\033[0m" << endl;
                Beep(500, 200);
                Sleep(800);
            }
            break;
        }
        case 'P': case 'p': {
            if (feedCurrent->prev) {
                feedCurrent = feedCurrent->prev;
                Beep(600, 100);
            }
            else {
                cout << "\033[33mFirst post.\033[0m" << endl;
                Beep(500, 200);
                Sleep(800);
            }
            break;
        }
        case 'L': case 'l': {
            if (feedCurrent->owner != currentUser->userName) {
                feedCurrent->likes++;
                updatePostLikesInAVL(feedCurrent->postID, feedCurrent->likes);
                sendLikeNotification(feedCurrent->owner, currentUser->userName);
                Beep(1000, 150);
                cout << "\033[1;32mLiked!\033[0m" << endl;
                Sleep(800);
                savePostsToFile();
            }
            else {
                cout << "\033[33mCannot like own post.\033[0m" << endl;
                Beep(700, 200);
                Sleep(800);
            }
            break;
        }
        }
    } while (ch != 'B' && ch != 'b');
    system("cls");
}
// ADD A STORY FOR THE CURRENT USER
void addStory() {
    if (!currentUser) {
        system("cls");
        cout << "\033[31mLogin first.\033[0m" << endl;
        return;
    }
    string cont;
    system("cls");
    cout << "\033[34mStory: \033[0m";
    getline(cin, cont);
    StoryNode* ns = new StoryNode(currentUser->userName, cont);
    if (!storiesHead) {
        storiesHead = ns;
        ns->next = storiesHead;
    }
    else {
        StoryNode* t = storiesHead;
        while (t->next != storiesHead) {
            t = t->next;
        }
        t->next = ns;
        ns->next = storiesHead;
    }
    Beep(950, 180);
    cout << "\033[1;32mStory added.\033[0m" << endl;
    saveStoriesToFile();
}
// VIEW ALL STORIES IN A CIRCULAR MANNER
void viewStories() {
    if (!storiesHead) {
        system("cls");
        cout << "\033[33mNo stories.\033[0m" << endl;
        return;
    }
    system("cls");
    cout << "\n\033[1;36m=== STORIES === (Enter to next, q to quit)\033[0m" << endl;
    StoryNode* cur = storiesHead;
    StoryNode* start = storiesHead;
    bool first = true;
    do {
        cout << "\n[\033[92m" << cur->userName << "\033[0m] " << cur->storyContent << endl;
        cout << "\033[33m[Enter] next, [q] quit: \033[0m";
        char ch;
        cin.get(ch);
        if (cin.peek() != '\n') {
            cin.ignore(1000, '\n');
        }
        if (ch == 'q' || ch == 'Q') {
            break;
        }
        cur = cur->next;
        if (!first && cur == start) {
            cout << "\033[33mAll stories viewed.\033[0m" << endl;
            break;
        }
        first = false;
    } while (true);
    system("cls");
}
// DELETE ALL STORIES OF A SPECIFIC USER
void deleteUserStories(string userName) {
    if (!storiesHead) {
        return;
    }
    if (storiesHead->next == storiesHead && storiesHead->userName == userName) {
        delete storiesHead;
        storiesHead = nullptr;
        return;
    }
    StoryNode* cur = storiesHead;
    StoryNode* prev = nullptr;
    do {
        if (cur->userName == userName) {
            if (prev) {
                prev->next = cur->next;
            }
            else {
                StoryNode* last = storiesHead;
                while (last->next != storiesHead) {
                    last = last->next;
                }
                last->next = cur->next;
                storiesHead = cur->next;
            }
            StoryNode* del = cur;
            cur = cur->next;
            delete del;
            if (cur == storiesHead) {
                break;
            }
        }
        else {
            prev = cur;
            cur = cur->next;
        }
    } while (cur != storiesHead);
    saveStoriesToFile();
}
// ADD A NOTIFICATION TO THE QUEUE
void enqueueNotification(string msg, string un) {
    NotificationNode* nn = new NotificationNode(msg, un, getCurrentTimestamp());
    if (!notificationRear) {
        notificationFront = notificationRear = nn;
    }
    else {
        notificationRear->next = nn;
        notificationRear = nn;
    }
    saveNotificationsToFile();
}
// REMOVE AND PROCESS THE FRONT NOTIFICATION 
void dequeueNotification() {
    if (!notificationFront) {
        system("cls");
        cout << "\033[33mNo notifications.\033[0m" << endl;
        return;
    }
    NotificationNode* tmp = notificationFront;
    Beep(800, 150);
    system("cls");
    cout << "\033[1;36mProcessing: To " << tmp->userName << " - " << tmp->message << " (" << tmp->timestamp << ")\033[0m" << endl;
    notificationFront = notificationFront->next;
    if (!notificationFront) {
        notificationRear = nullptr;
    }
    delete tmp;
    saveNotificationsToFile();
}
// PEEK AT THE FRONT NOTIFICATION WITHOUT REMOVING IT
void peekNotification() {
    if (!notificationFront) {
        system("cls");
        cout << "\033[33mNo notifications.\033[0m" << endl;
    }
    else {
        Beep(700, 120);
        cout << "\033[1;36mOldest: " << notificationFront->message << " for " << notificationFront->userName << "\033[0m" << endl;
    }
}
// DISPLAY ALL NOTIFICATIONS FOR THE CURRENT USER
void displayNotifications() {
    if (!currentUser) {
        system("cls");
        cout << "\033[31mLogin first.\033[0m" << endl;
        return;
    }
    if (!notificationFront) {
        system("cls");
        cout << "\033[33mNo notifications.\033[0m" << endl;
        return;
    }
    system("cls");
    cout << "\n\033[1;36m=== YOUR NOTIFICATIONS ===\033[0m" << endl;
    NotificationNode* t = notificationFront;
    int c = 0;
    while (t) {
        if (t->userName == currentUser->userName) {
            cout << "\033[93m" << ++c << ". \033[96m" << t->message << "\033[0m \033[90m(" << t->timestamp << ")\033[0m" << endl;
        }
        t = t->next;
    }
    if (!c) {
        cout << "\033[33mNone.\033[0m" << endl;
    }
}
// SEND A NOTIFICATION WHEN A POST IS LIKED
void sendLikeNotification(string owner, string liker) {
    enqueueNotification(liker + " liked your post.", owner);
}
// SEND A NOTIFICATION WHEN A MESSAGE IS RECEIVED
void sendMessageNotification(string receiver, string sender) {
    enqueueNotification("New message from " + sender, receiver);
}
// FIND AN EXISTING CONVERSATION BETWEEN TWO USERS
Conversation* findConversation(string a, string b) {
    Conversation* c = conversationsList;
    while (c) {
        if ((c->user1 == a && c->user2 == b) || (c->user1 == b && c->user2 == a)) {
            return c;
        }
        c = c->next;
    }
    return nullptr;
}
// GET OR CREATE A CONVERSATION BETWEEN TWO USERS
Conversation* getOrCreateConversation(string a, string b) {
    Conversation* ex = findConversation(a, b);
    if (ex) {
        return ex;
    }
    Conversation* nc = new Conversation();
    nc->user1 = a;
    nc->user2 = b;
    nc->next = conversationsList;
    conversationsList = nc;
    return nc;
}
// SEND A MESSAGE FROM CURRENT USER TO ANOTHER USER
void sendMessage() {
    if (!currentUser) {
        system("cls");
        cout << "\033[31mLogin first.\033[0m" << endl;
        return;
    }
    string rec, txt;
    cout << "\033[34mReceiver: \033[0m";
    cin >> rec;
    cin.ignore(1000, '\n');
    if (!checkUserExists(rec)) {
        system("cls");
        cout << "\033[31mNo such user.\033[0m" << endl;
        return;
    }
    if (rec == currentUser->userName) {
        system("cls");
        cout << "\033[31mCannot self message.\033[0m" << endl;
        return;
    }
    cout << "\033[34mMessage: \033[0m";
    getline(cin, txt);
    MessageNode* nm = new MessageNode(currentUser->userName, rec, txt, getCurrentTimestamp());
    Conversation* conv = getOrCreateConversation(currentUser->userName, rec);
    nm->next = conv->stackTop;
    conv->stackTop = nm;
    Beep(1000, 150);
    cout << "\033[1;32mSent.\033[0m" << endl;
    sendMessageNotification(rec, currentUser->userName);
    saveMessagesToFile();
}
// VIEW THE LATEST MESSAGE IN A CONVERSATION
void viewLatestMessage() {
    if (!currentUser) {
        system("cls");
        cout << "\033[31mLogin first.\033[0m" << endl;
        return;
    }
    string other;
    cout << "\033[34mWith whom: \033[0m";
    cin >> other;
    cin.ignore(1000, '\n');
    Conversation* conv = findConversation(currentUser->userName, other);
    if (!conv || !conv->stackTop) {
        cout << "\033[33mNo messages.\033[0m" << endl;
        return;
    }
    MessageNode* top = conv->stackTop;
    Beep(750, 120);
    cout << "\033[1;36mLatest from " << top->fromUser << ": " << top->text << " (" << top->timestamp << ")\033[0m" << endl;
}
// REMOVE THE LATEST MESSAGE FROM A CONVERSATION
void popMessage() {
    if (!currentUser) {
        system("cls");
        cout << "\033[31mLogin first.\033[0m" << endl;
        return;
    }
    string other;
    cout << "\033[34mWith whom: \033[0m";
    cin >> other;
    cin.ignore(1000, '\n');
    Conversation* conv = findConversation(currentUser->userName, other);
    if (!conv || !conv->stackTop) {
        cout << "\033[33mNo messages.\033[0m" << endl;
        return;
    }
    MessageNode* popped = conv->stackTop;
    conv->stackTop = conv->stackTop->next;
    Beep(800, 150);
    cout << "\033[33mPopped: " << popped->fromUser << ": " << popped->text << "\033[0m" << endl;
    delete popped;
    saveMessagesToFile();
}
// DISPLAY THE ENTIRE CONVERSATION HISTORY WITH ANOTHER USER
void displayConversation() {
    if (!currentUser) {
        system("cls");
        cout << "\033[31mLogin first.\033[0m" << endl;
        return;
    }
    string other;
    cout << "\033[34mWith whom: \033[0m";
    cin >> other;
    cin.ignore(1000, '\n');
    Conversation* conv = findConversation(currentUser->userName, other);
    if (!conv || !conv->stackTop) {
        cout << "\033[33mNo messages.\033[0m" << endl;
        return;
    }
    system("cls");
    cout << "\n\033[1;36m=== Chat with " << other << " ===\033[0m" << endl;
    MessageNode* m = conv->stackTop;
    int c = 0;
    while (m) {
        cout << "\033[93m" << ++c << ". \033[0m";
        if (m->fromUser == currentUser->userName) {
            cout << "\033[92mYou: \033[0m";
        }
        else {
            cout << "\033[96m" << m->fromUser << ": \033[0m";
        }
        cout << m->text << " \033[90m(" << m->timestamp << ")\033[0m" << endl;
        m = m->next;
    }
}
// DELETE ALL CONVERSATIONS INVOLVING A SPECIFIC USER
void deleteUserMessages(string userName) {
    Conversation* prev = nullptr;
    Conversation* cur = conversationsList;
    while (cur) {
        if (cur->user1 == userName || cur->user2 == userName) {
            MessageNode* m = cur->stackTop;
            while (m) {
                MessageNode* d = m;
                m = m->next;
                delete d;
            }
            if (prev) {
                prev->next = cur->next;
            }
            else {
                conversationsList = cur->next;
            }
            Conversation* del = cur;
            cur = cur->next;
            delete del;
        }
        else {
            prev = cur;
            cur = cur->next;
        }
    }
    saveMessagesToFile();
}
// GET HEIGHT OF AN AVL NODE 
int getHeight(AVLNode* n) {
    if (n != nullptr) {
        return n->height;
    }
    else {
        return 0;
    }
}
// GET BALANCE FACTOR OF AN AVL NODE
int getBalance(AVLNode* n) {
    if (n) {
        return getHeight(n->left) - getHeight(n->right);
    }
    else {
        return 0;
    }
}
// PERFORM RIGHT ROTATION ON AN AVL SUBTREE
AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    return x;
}
// PERFORM LEFT ROTATION ON AN AVL SUBTREE
AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    return y;
}
// INSERT A KEY-VALUE PAIR INTO THE AVL TREE
AVLNode* insertAVL(AVLNode* node, string key, int value) {
    if (!node) {
        return new AVLNode(key, value);
    }
    if (key < node->key) {
        node->left = insertAVL(node->left, key, value);
    }
    else if (key > node->key) {
        node->right = insertAVL(node->right, key, value);
    }
    else {
        node->value = value;
        return node;
    }
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    int bal = getBalance(node);
    if (bal > 1 && key < node->left->key) {
        return rightRotate(node);
    }
    if (bal < -1 && key > node->right->key) {
        return leftRotate(node);
    }
    if (bal > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (bal < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}
// SEARCH FOR A KEY IN THE AVL TREE
AVLNode* searchAVL(AVLNode* node, string key) {
    if (!node || node->key == key) {
        return node;
    }
    if (key < node->key) {
        return searchAVL(node->left, key);
    }
    return searchAVL(node->right, key);
}
// UPDATE THE VALUE OF AN EXISTING KEY, OR INSERT IF NOT FOUND
void updateAVLValue(AVLNode*& root, string key, int newVal) {
    AVLNode* n = searchAVL(root, key);
    if (n) {
        n->value = newVal;
    }
    else {
        root = insertAVL(root, key, newVal);
    }
}
// FIND THE NODE WITH MINIMUM KEY IN AN AVL SUBTREE
AVLNode* findMinNode(AVLNode* node) {
    while (node && node->left) {
        node = node->left;
    }
    return node;
}
// DELETE A KEY FROM THE AVL TREE
AVLNode* deleteAVL(AVLNode* root, string key) {
    if (!root) {
        return nullptr;
    }
    if (key < root->key) {
        root->left = deleteAVL(root->left, key);
    }
    else if (key > root->key) {
        root->right = deleteAVL(root->right, key);
    }
    else {
        if (!root->left || !root->right) {
            AVLNode* temp;
            if (root->left != nullptr) {
                temp = root->left;
            }
            else {
                temp = root->right;
            }
            if (!temp) {
                delete root;
                return nullptr;
            }
            else {
                *root = *temp;
                delete temp;
            }
        }
        else {
            AVLNode* temp = findMinNode(root->right);
            root->key = temp->key;
            root->value = temp->value;
            root->right = deleteAVL(root->right, temp->key);
        }
    }
    if (!root) {
        return root;
    }
    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    int bal = getBalance(root);
    if (bal > 1 && getBalance(root->left) >= 0) {
        return rightRotate(root);
    }
    if (bal > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (bal < -1 && getBalance(root->right) <= 0) {
        return leftRotate(root);
    }
    if (bal < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}
// INCREASE ACTIVITY COUNT FOR A USER IN THE USER AVL TREE
void updateUserActivityInAVL(string userName) {
    AVLNode* n = searchAVL(userAVLRoot, userName);
    int newVal;
    if (n) {
        newVal = n->value + 1;
    }
    else {
        newVal = 1;
    }
    updateAVLValue(userAVLRoot, userName, newVal);
}
// UPDATE LIKE COUNT FOR A POST IN THE POST AVL TREE
void updatePostLikesInAVL(string postID, int newLikes) {
    updateAVLValue(postAVLRoot, postID, newLikes);
}
// ADD A NEW POST TO THE POST AVL TREE
void addPostToAVL(string postID) {
    postAVLRoot = insertAVL(postAVLRoot, postID, 0);
}
// REMOVE A POST FROM THE POST AVL TREE
void removePostFromAVL(string postID) {
    postAVLRoot = deleteAVL(postAVLRoot, postID);
}
// REMOVE A USER FROM THE USER AVL TREE
void removeUserFromAVL(string userName) {
    userAVLRoot = deleteAVL(userAVLRoot, userName);
}
// RECURSIVE HELPER FOR GETTING TOP K ELEMENTS 
void getTopKHelper(AVLNode* root, int& cnt, int k, string title, bool& printed) {
    if (!root || cnt >= k) {
        return;
    }
    getTopKHelper(root->right, cnt, k, title, printed);
    if (cnt < k) {
        if (!printed) {
            cout << "\n\033[1;36m=== TOP " << k << " " << title << " ===\033[0m" << endl;
            printed = true;
        }
        cout << "\033[93m" << ++cnt << ". \033[96m" << root->key << "\033[0m - \033[92m" << root->value << " points\033[0m" << endl;
        getTopKHelper(root->left, cnt, k, title, printed);
    }
}
// DISPLAY TOP K HIGHEST-VALUE NODES FROM AVL TREE
void getTopK(AVLNode* root, int k, string title) {
    if (!root) {
        system("cls");
        cout << "\033[33mNo data for " << title << ".\033[0m" << endl;
        return;
    }
    int cnt = 0;
    bool printed = false;
    getTopKHelper(root, cnt, k, title, printed);
    if (!printed) {
        cout << "\033[33mNot enough data.\033[0m" << endl;
    }
    cout << endl;
}
// PRINT ALL NODES IN INORDER
void inorderPrint(AVLNode* root) {
    if (!root) {
        return;
    }
    inorderPrint(root->left);
    cout << "\033[34m* \033[96m" << root->key << "\033[0m - \033[92m" << root->value << "\033[0m" << endl;
    inorderPrint(root->right);
}
// DISPLAY ALL NODES SORTED BY KEY
void inorderDisplay(AVLNode* root, string title) {
    if (!root) {
        cout << "\033[33mNo data.\033[0m" << endl;
        return;
    }
    system("cls");
    cout << "\n\033[1;36m=== SORTED " << title << " ===\033[0m" << endl;
    inorderPrint(root);
    cout << endl;
}
// RECURSIVE HELPER FOR RANGE QUERY ON AVL TREE
void rangeQueryHelper(AVLNode* root, int minV, int maxV, bool& found) {
    if (!root) {
        return;
    }
    if (root->value >= minV && root->value <= maxV) {
        cout << "\033[34m* \033[96m" << root->key << "\033[0m - \033[92m" << root->value << "\033[0m" << endl;
        found = true;
    }
    rangeQueryHelper(root->left, minV, maxV, found);
    rangeQueryHelper(root->right, minV, maxV, found);
}
// DISPLAY NODES WHOSE VALUES FALL WITHIN A GIVEN RANGE
void rangeQuery(AVLNode* root, int minV, int maxV, string title) {
    if (!root) {
        cout << "\033[33mNo data.\033[0m" << endl;
        return;
    }
    cout << "\n\033[1;36m=== " << title << " range [" << minV << "," << maxV << "] ===\033[0m" << endl;
    bool found = false;
    rangeQueryHelper(root, minV, maxV, found);
    if (!found) {
        cout << "\033[33mNone.\033[0m" << endl;
    }
    cout << endl;
}
// DELETE ALL POSTS OF A SPECIFIC USER
void deleteAllUserPosts(string userName) {
    int idx = hashFunction(userName);
    UserNode* u = hashTable[idx];
    while (u && u->userName != userName) {
        u = u->next;
    }
    if (!u) {
        return;
    }
    PostNode* p = u->postsHead;
    while (p) {
        removePostFromAVL(p->postID);
        removeFromFeedList(p->postID);
        PostNode* d = p;
        p = p->next;
        delete d;
    }
    u->postsHead = nullptr;
    savePostsToFile();
}
// REMOVE A USER FROM THE FRIENDSHIP GRAPH 
void removeUserFromGraph(string userName) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        UserNode* u = hashTable[i];
        while (u) {
            removeFriendEdge(u, userName);
            u = u->next;
        }
    }
    int idx = hashFunction(userName);
    UserNode* u = hashTable[idx];
    while (u && u->userName != userName) {
        u = u->next;
    }
    if (u) {
        FriendNode* f = u->friendsHead;
        while (f) {
            FriendNode* d = f;
            f = f->next;
            delete d;
        }
        u->friendsHead = nullptr;
    }
    saveFriendsToFile();
}
// COMPLETE DELETION OF A USER FROM ALL MODULES
void completeUserDeletion(string userName) {
    system("cls");
    cout << "\n\033[1;31m--- Deleting user from all modules ---\033[0m" << endl;
    deleteAllUserPosts(userName);
    deleteUserStories(userName);
    deleteUserMessages(userName);
    removeUserFromGraph(userName);
    removeUserFromAVL(userName);
    Beep(800, 300);
    cout << "\033[1;32mUser data removed.\033[0m" << endl;
}
// DELETE THE CURRENT USER'S ACCOUNT AND ALL ASSOCIATED DATA
void deleteUser() {
    if (!currentUser) {
        system("cls");
        cout << "\033[31mLogin first.\033[0m" << endl;
        return;
    }
    char ch;
    cout << "\033[33mDelete account? (y/n): \033[0m";
    cin >> ch;
    cin.ignore(1000, '\n');
    if (ch != 'y' && ch != 'Y') {
        cout << "\033[33mCancelled.\033[0m" << endl;
        return;
    }
    string un = currentUser->userName;
    completeUserDeletion(un);
    int idx = hashFunction(un);
    UserNode* prev = nullptr;
    UserNode* cur = hashTable[idx];
    while (cur && cur->userName != un) {
        prev = cur;
        cur = cur->next;
    }
    if (cur) {
        if (prev) {
            prev->next = cur->next;
        }
        else hashTable[idx] = cur->next;
        delete cur;
    }
    currentUser = nullptr;
    Beep(900, 400);
    cout << "\033[1;32mAccount deleted. You are logged out.\033[0m" << endl;
    saveUsersToFile();
}
// FILE HANDLING FUNCTIONS
void saveUsersToFile() {
    ofstream file("users.txt");
    if (!file.is_open()) {
        return;
    }
    for (int i = 0; i < TABLE_SIZE; i++) {
        UserNode* temp = hashTable[i];
        while (temp) {
            file << temp->userName << "|" << temp->password << "|" << temp->email << "|" << temp->bio << "|" << temp->lastActive << endl;
            temp = temp->next;
        }
    }
    file.close();
}
void loadUsersFromFile() {
    ifstream file("users.txt");
    if (!file.is_open()) {
        return;
    }
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string un, pwd, em, bio, lastActive;
        getline(ss, un, '|');
        getline(ss, pwd, '|');
        getline(ss, em, '|');
        getline(ss, bio, '|');
        getline(ss, lastActive, '|');
        UserNode* nu = new UserNode(un, pwd, em, bio);
        nu->lastActive = lastActive;
        int idx = hashFunction(un);
        nu->next = hashTable[idx];
        hashTable[idx] = nu;
        userAVLRoot = insertAVL(userAVLRoot, un, 0);
    }
    file.close();
}
void saveFriendsToFile() {
    ofstream file("friends.txt");
    if (!file.is_open()) {
        return;
    }
    for (int i = 0; i < TABLE_SIZE; i++) {
        UserNode* u = hashTable[i];
        while (u) {
            FriendNode* f = u->friendsHead;
            while (f) {
                file << u->userName << "|" << f->friendUserName << endl;
                f = f->next;
            }
            u = u->next;
        }
    }
    file.close();
}
void loadFriendsFromFile() {
    ifstream file("friends.txt");
    if (!file.is_open()) {
        return;
    }
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string userName, friendName;
        getline(ss, userName, '|');
        getline(ss, friendName, '|');
        int idx = hashFunction(userName);
        UserNode* u = hashTable[idx];
        while (u && u->userName != userName) {
            u = u->next;
        }
        if (u) {
            addFriendEdge(u, friendName);
        }
    }
    file.close();
}
void savePostsToFile() {
    ofstream file("posts.txt");
    if (!file.is_open()) {
        return;
    }
    for (int i = 0; i < TABLE_SIZE; i++) {
        UserNode* u = hashTable[i];
        while (u) {
            PostNode* p = u->postsHead;
            while (p) {
                file << p->postID << "|" << p->content << "|" << p->owner << "|" << p->likes << endl;
                p = p->next;
            }
            u = u->next;
        }
    }
    file.close();
}
void loadPostsFromFile() {
    ifstream file("posts.txt");
    if (!file.is_open()) {
        return;
    }
    string line;
    int maxPostNum = 0;
    while (getline(file, line)) {
        stringstream ss(line);
        string pid, content, owner;
        int likes;
        getline(ss, pid, '|');
        getline(ss, content, '|');
        getline(ss, owner, '|');
        ss >> likes;
        int idx = hashFunction(owner);
        UserNode* u = hashTable[idx];
        while (u && u->userName != owner) {
            u = u->next;
        }
        if (u) {
            PostNode* np = new PostNode(pid, content, owner);
            np->likes = likes;
            np->next = u->postsHead;
            if (u->postsHead) u->postsHead->prev = np;
            u->postsHead = np;
            addToFeedList(np);
            addPostToAVL(pid);
            updatePostLikesInAVL(pid, likes);
            int postNum = stoi(pid.substr(4));
            if (postNum > maxPostNum) maxPostNum = postNum;
        }
    }
    postCounter = maxPostNum + 1;
    file.close();
}
void saveStoriesToFile() {
    ofstream file("stories.txt");
    if (!file.is_open()) {
        return;
    }
    if (storiesHead) {
        StoryNode* cur = storiesHead;
        do {
            file << cur->userName << "|" << cur->storyContent << endl;
            cur = cur->next;
        } while (cur != storiesHead);
    }
    file.close();
}
void loadStoriesFromFile() {
    ifstream file("stories.txt");
    if (!file.is_open()) {
        return;
    }
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string userName, storyContent;
        getline(ss, userName, '|');
        getline(ss, storyContent, '|');
        StoryNode* ns = new StoryNode(userName, storyContent);
        if (!storiesHead) {
            storiesHead = ns;
            ns->next = storiesHead;
        }
        else {
            StoryNode* t = storiesHead;
            while (t->next != storiesHead) {
                t = t->next;
            }
            t->next = ns;
            ns->next = storiesHead;
        }
    }
    file.close();
}
void saveMessagesToFile() {
    ofstream file("messages.txt");
    if (!file.is_open()) {
        return;
    }
    Conversation* conv = conversationsList;
    while (conv) {
        MessageNode* m = conv->stackTop;
        while (m) {
            file << conv->user1 << "|" << conv->user2 << "|" << m->fromUser << "|" << m->toUser << "|" << m->text << "|" << m->timestamp << endl;
            m = m->next;
        }
        conv = conv->next;
    }
    file.close();
}
void loadMessagesFromFile() {
    ifstream file("messages.txt");
    if (!file.is_open()) {
        return;
    }
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string user1, user2, fromUser, toUser, text, timestamp;
        getline(ss, user1, '|');
        getline(ss, user2, '|');
        getline(ss, fromUser, '|');
        getline(ss, toUser, '|');
        getline(ss, text, '|');
        getline(ss, timestamp, '|');
        Conversation* conv = getOrCreateConversation(user1, user2);
        MessageNode* nm = new MessageNode(fromUser, toUser, text, timestamp);
        nm->next = conv->stackTop;
        conv->stackTop = nm;
    }
    file.close();
}
void saveNotificationsToFile() {
    ofstream file("notifications.txt");
    if (!file.is_open()) {
        return;
    }
    NotificationNode* temp = notificationFront;
    while (temp) {
        file << temp->message << "|" << temp->userName << "|" << temp->timestamp << endl;
        temp = temp->next;
    }
    file.close();
}
void loadNotificationsFromFile() {
    ifstream file("notifications.txt");
    if (!file.is_open()) {
        return;
    }
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string message, userName, timestamp;
        getline(ss, message, '|');
        getline(ss, userName, '|');
        getline(ss, timestamp, '|');
        NotificationNode* nn = new NotificationNode(message, userName, timestamp);
        if (!notificationRear) {
            notificationFront = notificationRear = nn;
        }
        else {
            notificationRear->next = nn;
            notificationRear = nn;
        }
    }
    file.close();
}
void saveAllData() {
    saveUsersToFile();
    saveFriendsToFile();
    savePostsToFile();
    saveStoriesToFile();
    saveMessagesToFile();
    saveNotificationsToFile();
}
void loadAllData() {
    loadUsersFromFile();
    loadFriendsFromFile();
    loadPostsFromFile();
    loadStoriesFromFile();
    loadMessagesFromFile();
    loadNotificationsFromFile();
}
// DISPLAY THE USER MANAGEMENT MENU
void displayUserMenu() {
    system("cls");
    cout << "\n\033[1;36m========== USER MANAGEMENT ==========\033[0m" << endl;
    cout << "\033[33m1. Register\033[0m" << endl;
    cout << "\033[33m2. Login\033[0m" << endl;
    cout << "\033[33m3. Logout\033[0m" << endl;
    cout << "\033[33m4. Delete User\033[0m" << endl;
    cout << "\033[33m5. Search User\033[0m" << endl;
    cout << "\033[33m6. Display All\033[0m" << endl;
    cout << "\033[33m7. Back\033[0m" << endl;
    cout << "\033[34mChoice: \033[0m";
}
// HANDLE USER MANAGEMENT MODULE
void userManagementModule() {
    int choice;
    do {
        displayUserMenu();
        choice = getInt();
        switch (choice) {
        case 1: {
            Beep(850, 100);
            registerUser();
            break;
        }
        case 2: {
            Beep(850, 100);
            loginUser();
            break;
        }
        case 3: {
            Beep(850, 100);
            logoutUser();
            break;
        }
        case 4: {
            Beep(850, 100);
            deleteUser();
            break;
        }
        case 5: {
            Beep(850, 100);
            searchUser();
            break;
        }
        case 6: {
            Beep(850, 100);
            displayAllUsers();
            break;
        }
        case 7: {
            Beep(850, 100);
            system("cls");
            break;
        }
        default: {
            cout << "\033[31mInvalid Input.\033[0m" << endl;
            cout << "\033[34mTry Again\033[0m" << endl;
            Beep(700, 300);
            break;
        }
        }
    } while (choice != 7);
}
// DISPLAY THE SOCIAL NETWORK MENU
void displaySocialNetworkMenu() {
    system("cls");
    cout << "\n\033[1;36m========== SOCIAL NETWORK ==========\033[0m" << endl;
    cout << "\033[33m1. Add Friend\033[0m" << endl;
    cout << "\033[33m2. Remove Friend\033[0m" << endl;
    cout << "\033[33m3. View Friends\033[0m" << endl;
    cout << "\033[33m4. BFS from me\033[0m" << endl;
    cout << "\033[33m5. DFS from me\033[0m" << endl;
    cout << "\033[33m6. Back\033[0m" << endl;
    cout << "\033[34mChoice: \033[0m";
}
// HANDLE SOCIAL NETWORK MODULE
void socialNetworkModule() {
    int choice;
    do {
        displaySocialNetworkMenu();
        choice = getInt();
        switch (choice) {
        case 1: {
            Beep(850, 100);
            addFriend();
            break;
        }
        case 2: {
            Beep(850, 100);
            removeFriend();
            break;
        }
        case 3: {
            Beep(850, 100);
            displayFriends();
            break;
        }
        case 4: {
            Beep(850, 100);
            if (currentUser) {
                BFS(currentUser->userName);
            }
            else {
                cout << "\033[31mLogin first.\033[0m" << endl;
            }
            break;
        }
        case 5: {
            Beep(850, 100);
            if (currentUser) {
                string t;
                cout << "\033[34mTarget: \033[0m";
                cin >> t;
                cin.ignore(1000, '\n');
                DFS(currentUser->userName, t);
            }
            else {
                cout << "\033[31mLogin first.\033[0m" << endl;
            }
            break;
        }
        case 6: {
            Beep(850, 100);
            system("cls");
            break;
        }
        default: {
            cout << "\033[31mInvalid Input.\033[0m" << endl;
            cout << "\033[34mTry Again\033[0m" << endl;
            Beep(700, 300);
            break;
        }
        }
    } while (choice != 6);
}
// DISPLAY THE POSTS & FEED MENU
void displayPostsMenu() {
    system("cls");
    cout << "\n\033[1;36m========== POSTS & FEED ==========\033[0m" << endl;
    cout << "\033[33m1. Create Post\033[0m" << endl;
    cout << "\033[33m2. Delete Post\033[0m" << endl;
    cout << "\033[33m3. My Posts\033[0m" << endl;
    cout << "\033[33m4. View Feed\033[0m" << endl;
    cout << "\033[33m5. Like a Post\033[0m" << endl;
    cout << "\033[33m6. Back\033[0m" << endl;
    cout << "\033[34mChoice: \033[0m";
}
// HANDLE POSTS & FEED MODULE
void postsModule() {
    int choice;
    do {
        displayPostsMenu();
        choice = getInt();
        switch (choice) {
        case 1: {
            Beep(850, 100);
            createPost();
            break;
        }
        case 2: {
            Beep(850, 100);
            deletePost();
            break;
        }
        case 3: {
            Beep(850, 100);
            displayMyPosts();
            break;
        }
        case 4: {
            Beep(850, 100);
            navigateFeed();
            break;
        }
        case 5: {
            Beep(850, 100);
            likePost();
            break;
        }
        case 6: {
            Beep(850, 100);
            system("cls");
            break;
        }
        default: {
            cout << "\033[31mInvalid Input.\033[0m" << endl;
            cout << "\033[34mTry Again\033[0m" << endl;
            Beep(700, 300);
            break;
        }
        }
    } while (choice != 6);
}
// DISPLAY THE STORIES MENU
void displayStoriesMenu() {
    system("cls");
    cout << "\n\033[1;36m========== STORIES ==========\033[0m" << endl;
    cout << "\033[33m1. Add Story\033[0m" << endl;
    cout << "\033[33m2. View Stories\033[0m" << endl;
    cout << "\033[33m3. Back\033[0m" << endl;
    cout << "\033[34mChoice: \033[0m";
}
// HANDLE STORIES MODULE 
void storiesModule() {
    int choice;
    do {
        displayStoriesMenu();
        choice = getInt();
        switch (choice) {
        case 1: {
            Beep(850, 100);
            addStory();
            break;
        }
        case 2: {
            Beep(850, 100);
            viewStories();
            break;
        }
        case 3: {
            Beep(850, 100);
            system("cls");
            break;
        }
        default: {
            cout << "\033[31mInvalid Input.\033[0m" << endl;
            cout << "\033[34mTry Again\033[0m" << endl;
            Beep(700, 300);
            break;
        }
        }
    } while (choice != 3);
}
// DISPLAY THE NOTIFICATIONS MENU
void displayNotificationsMenu() {
    system("cls");
    cout << "\n\033[1;36m========== NOTIFICATIONS ==========\033[0m" << endl;
    cout << "\033[33m1. View Mine\033[0m" << endl;
    cout << "\033[33m2. Process Next\033[0m" << endl;
    cout << "\033[33m3. Peek\033[0m" << endl;
    cout << "\033[33m4. Back\033[0m" << endl;
    cout << "\033[34mChoice: \033[0m";
}
// HANDLE NOTIFICATIONS MODULE
void notificationsModule() {
    int choice;
    do {
        displayNotificationsMenu();
        choice = getInt();
        switch (choice) {
        case 1: {
            Beep(850, 100);
            displayNotifications();
            break;
        }
        case 2: {
            Beep(850, 100);
            dequeueNotification();
            break;
        }
        case 3: {
            Beep(850, 100);
            peekNotification();
            break;
        }
        case 4: {
            Beep(850, 100);
            system("cls");
            break;
        }
        default: {
            cout << "\033[31mInvalid Input.\033[0m" << endl;
            cout << "\033[34mTry Again\033[0m" << endl;
            Beep(700, 300);
            break;
        }
        }
    } while (choice != 4);
}
// DISPLAY THE MESSAGING MENU
void displayMessagingMenu() {
    system("cls");
    cout << "\n\033[1;36m========== MESSAGING ==========\033[0m" << endl;
    cout << "\033[33m1. Send Message\033[0m" << endl;
    cout << "\033[33m2. View Latest\033[0m" << endl;
    cout << "\033[33m3. Pop Latest\033[0m" << endl;
    cout << "\033[33m4. View Conversation\033[0m" << endl;
    cout << "\033[33m5. Back\033[0m" << endl;
    cout << "\033[34mChoice: \033[0m";
}
// HANDLE MESSAGING MODULE
void messagingModule() {
    int choice;
    do {
        displayMessagingMenu();
        choice = getInt();
        switch (choice) {
        case 1: {
            Beep(850, 100);
            sendMessage();
            break;
        }
        case 2: {
            Beep(850, 100);
            viewLatestMessage();
            break;
        }
        case 3: {
            Beep(850, 100);
            popMessage();
            break;
        }
        case 4: {
            Beep(850, 100);
            displayConversation();
            break;
        }
        case 5: {
            Beep(850, 100);
            system("cls");
            break;
        }
        default: {
            cout << "\033[31mInvalid Input.\033[0m" << endl;
            cout << "\033[34mTry Again\033[0m" << endl;
            Beep(700, 300);
            break;
        }
        }
    } while (choice != 5);
}
// DISPLAY THE ANALYTICS MENU
void displayAnalyticsMenu() {
    system("cls");
    cout << "\n\033[1;36m========== ANALYTICS ==========\033[0m" << endl;
    cout << "\033[33m1. Top Active Users\033[0m" << endl;
    cout << "\033[33m2. Most Liked Posts\033[0m" << endl;
    cout << "\033[33m3. Sorted Users\033[0m" << endl;
    cout << "\033[33m4. Sorted Posts\033[0m" << endl;
    cout << "\033[33m5. Range Users\033[0m" << endl;
    cout << "\033[33m6. Range Posts\033[0m" << endl;
    cout << "\033[33m7. Back\033[0m" << endl;
    cout << "\033[34mChoice: \033[0m";
}
// HANDLE ANALYTICS MODULE
void analyticsModule() {
    int choice, mn, mx;
    do {
        displayAnalyticsMenu();
        choice = getInt();
        switch (choice) {
        case 1: {
            Beep(850, 100);
            getTopK(userAVLRoot, 5, "ACTIVE USERS");
            break;
        }
        case 2: {
            Beep(850, 100);
            getTopK(postAVLRoot, 5, "LIKED POSTS");
            break;
        }
        case 3: {
            Beep(850, 100);
            inorderDisplay(userAVLRoot, "USERS");
            break;
        }
        case 4: {
            Beep(850, 100);
            inorderDisplay(postAVLRoot, "POSTS");
            break;
        }
        case 5: {
            Beep(850, 100);
            cout << "\033[34mMin activity: \033[0m";
            mn = getInt();
            cout << "\033[34mMax: \033[0m";
            mx = getInt();
            rangeQuery(userAVLRoot, mn, mx, "USERS");
            break;
        }
        case 6: {
            Beep(850, 100);
            cout << "\033[34mMin likes: \033[0m";
            mn = getInt();
            cout << "\033[34mMax: \033[0m";
            mx = getInt();
            rangeQuery(postAVLRoot, mn, mx, "POSTS");
            break;
        }
        case 7: {
            Beep(850, 100);
            system("cls");
            break;
        }
        default: {
            cout << "\033[31mInvalid Input.\033[0m" << endl;
            cout << "\033[34mTry Again\033[0m" << endl;
            Beep(700, 300);
            break;
        }
        }
    } while (choice != 7);
}
// DISPLAY THE MAIN MENU WITH CURRENT LOGIN STATUS
void displayMainMenu() {
    if (currentUser) {
        cout << "\033[1;36m===========================================\033[0m" << endl;
        cout << "\033[92m   Logged in as: " << currentUser->userName << "\033[0m" << endl;
    }
    else {
        cout << "\033[1;36m===========================================\033[0m" << endl;
        cout << "\033[91m   Status: NOT LOGGED IN\033[0m" << endl;
    }
    cout << "\033[1;36m===========================================\033[0m" << endl;
    cout << endl;
    cout << "\033[33m1. User Management\033[0m" << endl;
    cout << "\033[33m2. Social Network\033[0m" << endl;
    cout << "\033[33m3. Posts & Feed\033[0m" << endl;
    cout << "\033[33m4. Stories\033[0m" << endl;
    cout << "\033[33m5. Notifications\033[0m" << endl;
    cout << "\033[33m6. Analytics\033[0m" << endl;
    cout << "\033[33m7. Messaging\033[0m" << endl;
    cout << "\033[33m8. Exit\033[0m" << endl;
    cout << endl;
    cout << "\033[34mChoice: \033[0m";
}
// MAIN 
int main() {
    initializeHashTable();
    loadAllData();
    system("cls");
    Beep(1200, 300);
    cout << "\033[1;36m===========================================\033[0m" << endl;
    cout << "\033[1;32m   WELCOME TO FAST SOCIAL MEDIA PLATFORM\033[0m" << endl;
    int choice;
    do {
        displayMainMenu();
        choice = getInt();
        switch (choice) {
        case 1: {
            Beep(850, 100);
            userManagementModule();
            break;
        }
        case 2: {
            Beep(850, 100);
            socialNetworkModule();
            break;
        }
        case 3: {
            Beep(850, 100);
            postsModule();
            break;
        }
        case 4: {
            Beep(850, 100);
            storiesModule();
            break;
        }
        case 5: {
            Beep(850, 100);
            notificationsModule();
            break;
        }
        case 6: {
            Beep(850, 100);
            analyticsModule();
            break;
        }
        case 7: {
            Beep(850, 100);
            messagingModule();
            break;
        }
        case 8: {
            Beep(850, 100);
            saveAllData();
            system("cls");
            Beep(1000, 500);
            cout << "\n\033[35mGoodbye!\033[0m" << endl;
            cout << endl;
            break;
        }
        default: {
            cout << "\033[31mInvalid Input.\033[0m" << endl;
            cout << "\033[34mTry Again\033[0m" << endl;
            Beep(700, 300);
            break;
        }
        }
    } while (choice != 8);
    return 0;
}
