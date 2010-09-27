#pragma once

// Registry setting value constants - we can't be duplicating string literals all over the place
// so instead they live here, and we just refer to their constant values in the app.
static const char PROTECTEDFOLDERSPROMPTPUP[] = "ProtectedFoldersPromptPup"; // Whether the user wants to be asked before decrypting protected folders
static const char PROTECTEDFOLDERSPROMPTPDN[] = "ProtectedFoldersPromptPdn"; // Whether the user wants to be asked before encrypted protected folders

// These are the hotkey settings for text encryption chosen by the user (if any)
static const char UTEENCRYPTUSECTRL[] = "UteEncryptUseCtrl";
static const char UTEENCRYPTUSEALT[] = "UteEncryptUseAlt";
static const char UTEENCRYPTHOTKEY[] = "UteEncryptHotkey";
static const char UTEDECRYPTUSECTRL[] = "UteDecryptUseCtrl";
static const char UTEDECRYPTUSEALT[] = "UteDecryptUseAlt";
static const char UTEDECRYPTHOTKEY[] = "UteDecryptHotkey";

static const char PFRLASTACTION[] = "PfrLastAction"; // This is the last selection option if the user has selected the protected folders prompt.
static const char AUTOUPDATECHECK[] = "AutoUpdateCheck"; // Whether the user wants cedecrypt to automatically check online for updates
static const char LASTUPDATECHECK[] = "LastUpdateCheck"; // The date of the last update check if automatic updates is turned on. We check just once a day
static const char SELECTEDALGORITHM[] = "SelectedAlgorithm"; // The currently selected encryption algorithm
