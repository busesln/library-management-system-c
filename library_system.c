#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define ISBN_LEN 14
#define NAME_LEN 20 // yazar/ogrenci isim/soyisimlerimi icin
#define BOOK_NAME_LEN 50 //kitap adi icin
#define STATUS_LEN 20 //RAFTA veya ogrenci numarasi icin
#define DATE_LEN 11 //DD.MM.YYYY

// Structures (Identical to your definitions)
typedef struct Yazar
{
    int id;
    char name[NAME_LEN];
    char surName[NAME_LEN];
    struct Yazar *next;
} YAZAR;

typedef struct Ogrenci
{
    char name[NAME_LEN];
    char surName[NAME_LEN];
    int libraryScore; //baslangicta 100 olacak
    int number;
    struct Ogrenci *next;
    struct Ogrenci *prev;
} OGRENCI;

typedef struct KitapCopy
{
    char label[ISBN_LEN + 5]; //isbn_1 isbn_2 gibi (ISBN + '_' + sayı + null)
    char status[STATUS_LEN]; //RAFTA veya ogrenci numarasi
    struct KitapCopy *next;
} KITAPCOPY;

typedef struct Kitap
{
    char name[BOOK_NAME_LEN];
    char isbn[ISBN_LEN];
    int copyCount;
    struct KitapCopy *copies; //kitap kopyalarının basi
    struct Kitap *next;
} KITAP;

typedef struct KitapYazarRel
{
    char isbn[ISBN_LEN];
    int yazarID;
} KITAPYAZAR_REL;

typedef struct OduncKaydi
{
    int OgrenciNumber;
    char kitapEtiketNo[ISBN_LEN + 5];
    int islemTuru; // 0 odunc alma 1 geri verme
    char tarih[DATE_LEN];
    struct OduncKaydi *next;
} ODUNCKAYDI;

// --- File Name Constants (to be passed as arguments) ---
// These will be defined in main and passed to functions.

// Function pointer prototypes (Identical)
typedef int (*CompareFunc)(const void *node_data, const void *key);
typedef void* (*GetNextNodeFunc)(void *node_data);

// Forward declarations for all functions
// Helper function to remove newline characters safely
void removeNewline(char *str);

// Yazar functions
void saveYazarlar(YAZAR *head, const char *filename);
void loadYazarlar(YAZAR **head, const char *filename);
void reindexYazarlar(YAZAR *head);
void insertYazar(YAZAR **head, const char *yazarlar_file);
void deleteYazar(YAZAR **head, KITAPYAZAR_REL **kyRelations, int *kyCount, const char *yazarlar_file, const char *kitapyazar_file);
void updateYazar(YAZAR *head, const char *yazarlar_file);
void listYazarlar(YAZAR *head);
YAZAR* findYazarById(YAZAR* head, int id);
YAZAR* findYazarByName(YAZAR* head, const char* name, const char* surname);
void listYazarInfoAndBooksById(YAZAR *yazarList, KITAP *kitapList, KITAPYAZAR_REL *relations, int count);
void listYazarInfoAndBooksByName(YAZAR *yazarList, KITAP *kitapList, KITAPYAZAR_REL *relations, int count);


// Ogrenci functions
void saveOgrenciler(OGRENCI *head, const char *filename);
void loadOgrenciler(OGRENCI **head, const char *filename);
OGRENCI *findOgrenciByNumber(OGRENCI *head, int number);
OGRENCI* findOgrenciByName(OGRENCI* head, const char* name, const char* surname);
void insertOgrenci(OGRENCI **head, const char *ogrenciler_file);
void deleteOgrenci(OGRENCI **head, const char *ogrenciler_file);
void updateOgrenci(OGRENCI *head, const char *ogrenciler_file);
void listOgrenciler(OGRENCI *head);
void listStudentInfoAndBorrowsByNumber(OGRENCI *ogrenciList, ODUNCKAYDI *oduncList);
void listStudentInfoAndBorrowsByName(OGRENCI *ogrenciList, ODUNCKAYDI *oduncList);


// Kitap and KitapCopy functions
void freeKitapCopies(KITAPCOPY *head);
void saveKitaplar(KITAP *head, const char *filename);
KITAP *findKitapByISBN(KITAP *head, const char *isbn);
KITAP* findKitapByName(KITAP* head, const char* name);
void loadKitaplar(KITAP **head, ODUNCKAYDI *oduncListHead, const char *filename);
void addBook(KITAP **head, const char *kitaplar_file);
void deleteKitap(KITAP **head, KITAPYAZAR_REL **kyRelations, int *kyCount, const char *kitaplar_file, const char *kitapyazar_file);
void updateKitap(KITAP *head, const char *kitaplar_file);
void listAllKitaplarWithCopies(KITAP *head);
void listKitapInfoByName(KITAP *kitapList);


// KitapYazarRel functions
void saveKitapYazarRelations(KITAPYAZAR_REL *relations, int count, const char *filename);
void loadKitapYazarRelations(KITAPYAZAR_REL **relations, int *count, const char *filename);
void addKitapYazarRelation(KITAPYAZAR_REL **relations, int *count, KITAP *kitapList, YAZAR *yazarList, const char *kitapyazar_file);
void listKitapYazarRelations(KITAPYAZAR_REL *relations, int count);
void updateBookAuthors(KITAP *kitapList, YAZAR *yazarList, KITAPYAZAR_REL **relations, int *count, const char *kitapyazar_file);


// OduncKaydi functions
void freeOduncList(ODUNCKAYDI *head);
void saveOduncKayitlari(ODUNCKAYDI *head, const char *filename);
void loadOduncKayitlari(ODUNCKAYDI **head, const char *filename);
void borrowKitap(OGRENCI *ogrenciList, KITAP *kitapList, ODUNCKAYDI **oduncListHead, const char *odunctakip_file, const char *ogrenciler_file);
void returnKitap(OGRENCI *ogrenciList, KITAP *kitapList, ODUNCKAYDI **oduncListHead, const char *odunctakip_file, const char *ogrenciler_file);

// Listing/Reporting Functions
void listStudentsWhoHaventReturnBooks(KITAP *kitapList, OGRENCI *ogrenciList);
void listPenalizedStudents(OGRENCI *head);
void listBooksOnShelf(KITAP *kitapList);
void listOverdueBooks(ODUNCKAYDI *oduncList, KITAP *kitapList, OGRENCI *ogrenciList);

// Generic findItemInList and helpers (Identical, but ensure const correctness in comparators)
void* findItemInList(void *head, void *key, CompareFunc compare, GetNextNodeFunc get_next);
int compareYazarById(const void *node_data, const void *key);
int compareOgrenciByNumber(const void *node_data, const void *key);
int compareKitapByISBN(const void *node_data, const void *key);
void* getNextYazar(void *node_data);
void* getNextOgrenci(void *node_data);
void* getNextKitap(void *node_data);

// Date functions (Identical)
void getCurrentDate(char *dateString);
int parseDate(const char *dateStr, int *day, int *month, int *year);
int calculateDaysDifference(const char* dateStr1, const char* dateStr2);


// --- Function Implementations ---

void removeNewline(char *str) {
    if (str == NULL) return;
    str[strcspn(str, "\r\n")] = 0;
}

// --- Generic List Search ---
void* findItemInList(void *head, void *key, CompareFunc compare, GetNextNodeFunc get_next)
{
    void *current = head;
    while(current != NULL){
        if(compare(current, key) == 0){ // 0 means match
            return current; // Return as void*
        }
        current = get_next(current);
    }
    return NULL;
}

// Specific comparison functions for findItemInList
int compareYazarById(const void *node_data, const void *key)
{
    const YAZAR *yazar = (const YAZAR *)node_data;
    const int *id = (const int *)key;
    return (yazar->id == *id) ? 0 : 1;
}

int compareOgrenciByNumber(const void *node_data, const void *key)
{
    const OGRENCI *ogrenci = (const OGRENCI *)node_data;
    const int *number = (const int *)key;
    return (ogrenci->number == *number) ? 0 : 1;
}

int compareKitapByISBN(const void *node_data, const void *key)
{
    const KITAP *kitap = (const KITAP *)node_data;
    const char *isbn_key = (const char *)key;
    return strcmp(kitap->isbn, isbn_key);
}

// Specific get_next functions for findItemInList
void* getNextYazar(void *node_data) { return ((YAZAR*)node_data)->next; }
void* getNextOgrenci(void *node_data) { return ((OGRENCI*)node_data)->next; }
void* getNextKitap(void *node_data) { return ((KITAP*)node_data)->next; }


// --- Date Functions ---
void getCurrentDate(char *dateString)
{
    time_t t = time(NULL);
    struct tm tm_info = *localtime(&t);
    sprintf(dateString,"%02d.%02d.%04d", tm_info.tm_mday, tm_info.tm_mon + 1, tm_info.tm_year + 1900);
}

int parseDate(const char *dateStr, int *day, int *month, int *year) // Made dateStr const
{
    if(sscanf(dateStr, "%d.%d.%d", day, month, year) == 3){
        return 1;
    }
    return 0;
}

int calculateDaysDifference(const char* dateStr1, const char* dateStr2) // Made dateStr const
{
    double difference_seconds;
    struct tm tm1 = {0}, tm2 = {0}; // Initialize to zero
    time_t t1, t2;

    // Use parseDate for robustness if needed, or stick to sscanf if format is guaranteed
    if(sscanf(dateStr1, "%d.%d.%d", &tm1.tm_mday, &tm1.tm_mon, &tm1.tm_year) != 3 ||
        sscanf(dateStr2, "%d.%d.%d", &tm2.tm_mday, &tm2.tm_mon, &tm2.tm_year) != 3){
        printf("Hata: Tarih formatı DD.MM.YYYY olmalıdır ('%s', '%s').\n", dateStr1, dateStr2);
        return 0; // Indicate error
    }

    tm1.tm_mon -= 1;
    tm1.tm_year -= 1900;
    tm2.tm_mon -= 1;
    tm2.tm_year -= 1900;

    // Ensure other fields are 0 for mktime consistency
    tm1.tm_hour = 0; tm1.tm_min = 0; tm1.tm_sec = 0; tm1.tm_isdst = -1;
    tm2.tm_hour = 0; tm2.tm_min = 0; tm2.tm_sec = 0; tm2.tm_isdst = -1;


    t1 = mktime(&tm1);
    t2 = mktime(&tm2);

    if(t1 == (time_t)-1 || t2 == (time_t)-1){
        printf("Hata: Tarihler time_t formatına dönüştürülemedi.\n");
        return 0; // Indicate error, perhaps a very large number to signify error if 0 is a valid difference
    }

    difference_seconds = difftime(t2, t1);
    return (int)(difference_seconds / (60 * 60 * 24));
}

// --- Yazar Functions ---
void saveYazarlar(YAZAR *head, const char *filename)
{
    YAZAR *current = head;
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        printf("Hata: %s dosyası yazmak için açılamadı!\n", filename);
        return;
    }
    while(current != NULL){
        fprintf(f, "%d,%s,%s\n", current->id, current->name, current->surName);
        current = current->next;
    }
    fclose(f);
}

void loadYazarlar(YAZAR **head, const char *filename)
{
    YAZAR *newYazar, *tail = NULL;
    char line[100]; // Increased buffer slightly for safety
    FILE *f = fopen(filename, "r");

    *head = NULL; // Initialize head to NULL

    if(f == NULL){
        // printf("%s dosyası bulunamadı. Yazar listesi boş başlatılıyor.\n", filename); // File not found is not an error for initial load
        return;
    }

    while(fgets(line, sizeof(line), f)){
        newYazar = (YAZAR *)malloc(sizeof(YAZAR));
        if(newYazar == NULL){
            printf("Hata: Yazar için bellek ayrılamadı.\n");
            fclose(f);
            // Free already loaded authors to prevent memory leak
            YAZAR *temp;
            while(*head != NULL) {
                temp = *head;
                *head = (*head)->next;
                free(temp);
            }
            return;
        }
        removeNewline(line);

        if(sscanf(line, "%d,%[^,],%[^\n]", &newYazar->id, newYazar->name, newYazar->surName) == 3){
            removeNewline(newYazar->name); // Ensure no newlines in fields
            removeNewline(newYazar->surName);
            newYazar->next = NULL;
            if(*head == NULL){
                *head = newYazar;
                tail = newYazar;
            } else {
                tail->next = newYazar;
                tail = newYazar;
            }
        } else {
            printf("Uyarı: %s dosyasındaki satır okunamadı: %s\n", filename, line);
            free(newYazar); // Free if parsing failed
        }
    }
    fclose(f);
}

void reindexYazarlar(YAZAR *head)
{
    YAZAR *current = head;
    int currentId = 1;
    while(current != NULL){
        current->id = currentId;
        currentId++;
        current = current->next;
    }
}

void insertYazar(YAZAR **head, const char *yazarlar_file)
{
    YAZAR *newYazar = (YAZAR *) malloc(sizeof(YAZAR));
    YAZAR *tmp;

    if(newYazar == NULL){
        printf("Hata: Yeni yazar için bellek ayrılamadı.\n");
        return;
    }

    printf("Yazarın adını girin: ");
    fgets(newYazar->name, NAME_LEN, stdin);
    removeNewline(newYazar->name); // Corrected: Apply to name

    printf("Yazarın soyadını girin: ");
    fgets(newYazar->surName, NAME_LEN, stdin);
    removeNewline(newYazar->surName); // Correct for surName

    newYazar->next = NULL;

    if(*head == NULL) {
        *head = newYazar;
    } else {
        tmp = *head;
        while(tmp->next != NULL){
            tmp = tmp->next;
        }
        tmp->next = newYazar;
    }
    reindexYazarlar(*head); // Re-index to assign new ID
    saveYazarlar(*head, yazarlar_file);
    printf("Yazar eklendi: %s %s [ID: %d]\n", newYazar->name, newYazar->surName, newYazar->id);
}

void deleteYazar(YAZAR **head, KITAPYAZAR_REL **kyRelations, int *kyCount, const char *yazarlar_file, const char *kitapyazar_file)
{
    YAZAR *current, *prev;
    int deleteId;
    int i;

    if(*head == NULL) {
        printf("Yazar listesi boş. Silinecek bir şey yok.\n");
        return;
    }

    printf("Silmek istediğiniz yazarın ID'sini girin: ");
    if (scanf("%d", &deleteId) != 1) {
        printf("Geçersiz ID girişi.\n");
        while(getchar() != '\n'); // Clear buffer
        return;
    }
    getchar(); // Consume newline

    current = *head;
    prev = NULL;

    // Find the author node
    while (current != NULL && current->id != deleteId) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) { // Not found
        printf("ID'si %d olan yazar bulunamadı.\n", deleteId);
        return;
    }

    // Found: current is the node to delete, prev is its predecessor
    if (prev == NULL) { // Deleting the head node
        *head = current->next;
    } else {
        prev->next = current->next;
    }
    printf("Yazar %s %s (ID: %d) silindi.\n", current->name, current->surName, current->id);
    free(current); // Free the correct node

    reindexYazarlar(*head); // Re-index IDs after deletion
    saveYazarlar(*head, yazarlar_file);

    // Update KitapYazar relations: set yazarID to -1 for the deleted author
    int relationsUpdated = 0;
    for (i = 0; i < *kyCount; i++) {
        if ((*kyRelations)[i].yazarID == deleteId) {
            (*kyRelations)[i].yazarID = -1; // Mark as deleted
            relationsUpdated = 1;
        }
    }
    if (relationsUpdated) {
        saveKitapYazarRelations(*kyRelations, *kyCount, kitapyazar_file);
        printf("KitapYazar ilişkileri, silinen yazar ID %d için güncellendi (ID -1 olarak ayarlandı).\n", deleteId);
    }
}

void updateYazar(YAZAR *head, const char *yazarlar_file)
{
    YAZAR *targetAuthor;
    int id;
    char newName[NAME_LEN];
    char newSurname[NAME_LEN];

    if(head == NULL){
        printf("Yazar listesi boş. Güncellenecek bir şey yok.\n");
        return;
    }
    printf("Güncellemek istediğiniz yazarın ID'sini girin: ");
    if (scanf("%d", &id) != 1) {
        printf("Geçersiz ID girişi.\n");
        while(getchar() != '\n');
        return;
    }
    getchar();

    targetAuthor = findYazarById(head, id);

    if(targetAuthor == NULL){
        printf("ID'si %d olan yazar bulunamadı.\n", id);
        return;
    }

    printf("Mevcut ad: %s\n", targetAuthor->name);
    printf("Yeni adı girin (değiştirmek istemiyorsanız boş bırakın): ");
    fgets(newName, NAME_LEN, stdin);
    removeNewline(newName);
    if(strlen(newName) > 0){
        strcpy(targetAuthor->name, newName);
    }

    printf("Mevcut soyad: %s\n", targetAuthor->surName);
    printf("Yeni soyadı girin (değiştirmek istemiyorsanız boş bırakın): ");
    fgets(newSurname, NAME_LEN, stdin);
    removeNewline(newSurname); // Added newline removal
    if(strlen(newSurname) > 0){
        strcpy(targetAuthor->surName, newSurname);
    }

    saveYazarlar(head, yazarlar_file);
    printf("Yazar başarıyla güncellendi.\n");
    printf("Güncellenmiş ad: %s, soyad: %s\n", targetAuthor->name, targetAuthor->surName);
}

void listYazarlar(YAZAR *head)
{
    YAZAR *current = head;
    if(head == NULL){
        printf("Görüntülenecek yazar yok.\n");
        return;
    }
    printf("\n--- Yazar Listesi ---\n");
    while(current != NULL){
        printf("ID: %d, Ad: %s, Soyad: %s\n", current->id, current->name, current->surName);
        current = current->next;
    }
    printf("-------------------\n");
}

YAZAR* findYazarById(YAZAR* head, int id) {
    // Can use the generic findItemInList or a direct implementation
    YAZAR* current = head;
    while (current != NULL) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

YAZAR* findYazarByName(YAZAR* head, const char* name, const char* surname) {
    YAZAR* current = head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0 && strcmp(current->surName, surname) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}


void listYazarInfoAndBooksById(YAZAR *yazarList, KITAP *kitapList, KITAPYAZAR_REL *relations, int count) {
    int yazarIdInput;
    YAZAR *yazar;
    int i;
    int booksFoundForAuthor = 0;
    KITAP *kitap;

    printf("Bilgilerini ve kitaplarını görüntülemek istediğiniz yazarın ID'sini girin: ");
    if (scanf("%d", &yazarIdInput) != 1) {
        printf("Geçersiz Yazar ID girişi.\n");
        while (getchar() != '\n');
        return;
    }
    getchar();

    yazar = findYazarById(yazarList, yazarIdInput);

    if (yazar == NULL) {
        printf("%d ID'li yazar bulunamadı.\n", yazarIdInput);
        return;
    }

    printf("\n--- Yazar Bilgileri ---\n");
    printf("ID: %d, Ad: %s, Soyad: %s\n", yazar->id, yazar->name, yazar->surName);
    printf("Yazara Ait Kitaplar:\n");

    for (i = 0; i < count; i++) {
        if (relations[i].yazarID == yazar->id && relations[i].yazarID != -1) {
            kitap = findKitapByISBN(kitapList, relations[i].isbn);
            if (kitap != NULL) {
                printf("  - Kitap Adı: %s, ISBN: %s\n", kitap->name, kitap->isbn);
                booksFoundForAuthor = 1;
            }
        }
    }

    if (!booksFoundForAuthor) {
        printf("  Bu yazara ait kayıtlı kitap bulunmamaktadır veya yazar ilişkilerden silinmiş.\n");
    }
    printf("-----------------------\n");
}

void listYazarInfoAndBooksByName(YAZAR *yazarList, KITAP *kitapList, KITAPYAZAR_REL *relations, int count) {
    char name[NAME_LEN], surname[NAME_LEN];
    YAZAR *yazar;
    int i;
    int booksFoundForAuthor = 0;
    KITAP *kitap;

    printf("Yazarın adını girin: ");
    fgets(name, NAME_LEN, stdin);
    removeNewline(name);
    printf("Yazarın soyadını girin: ");
    fgets(surname, NAME_LEN, stdin);
    removeNewline(surname);

    yazar = findYazarByName(yazarList, name, surname);

    if (yazar == NULL) {
        printf("Yazar %s %s bulunamadı.\n", name, surname);
        return;
    }

    printf("\n--- Yazar Bilgileri ---\n");
    printf("ID: %d, Ad: %s, Soyad: %s\n", yazar->id, yazar->name, yazar->surName);
    printf("Yazara Ait Kitaplar:\n");

    for (i = 0; i < count; i++) {
        if (relations[i].yazarID == yazar->id && relations[i].yazarID != -1) {
            kitap = findKitapByISBN(kitapList, relations[i].isbn);
            if (kitap != NULL) {
                printf("  - Kitap Adı: %s, ISBN: %s\n", kitap->name, kitap->isbn);
                booksFoundForAuthor = 1;
            }
        }
    }
    if (!booksFoundForAuthor) {
        printf("  Bu yazara ait kayıtlı kitap bulunmamaktadır.\n");
    }
    printf("-----------------------\n");
}


// --- Ogrenci Functions ---
void saveOgrenciler(OGRENCI *head, const char *filename)
{
    OGRENCI *current = head;
    FILE *f = fopen(filename, "w");
    if(f == NULL){
        printf("Hata: %s dosyası yazmak için açılamadı!\n", filename);
        return;
    }
    while(current != NULL){
        fprintf(f, "%s,%s,%d,%d\n", current->name, current->surName, current->libraryScore, current->number);
        current = current->next;
    }
    fclose(f);
}

void loadOgrenciler(OGRENCI **head, const char *filename)
{
    OGRENCI *newOgrenci, *tail = NULL;
    char line[150];
    FILE *f = fopen(filename, "r");

    *head = NULL; // Initialize head

    if(f == NULL){
        // printf("%s dosyası bulunamadı. Öğrenci listesi boş başlatılıyor.\n", filename);
        return;
    }

    while(fgets(line, sizeof(line), f)){
        newOgrenci = (OGRENCI *)malloc(sizeof(OGRENCI));
        if(newOgrenci == NULL){
            printf("Hata: Öğrenci için bellek ayrılamadı.\n");
            fclose(f);
            // Free already loaded students
            OGRENCI *temp;
            while(*head != NULL) {
                temp = *head;
                *head = (*head)->next;
                if ((*head) != NULL) (*head)->prev = NULL; // Not strictly necessary here as we are freeing all
                free(temp);
            }
            return;
        }
        removeNewline(line);

        if(sscanf(line, "%[^,],%[^,],%d,%d", newOgrenci->name, newOgrenci->surName, &newOgrenci->libraryScore, &newOgrenci->number) == 4){
            removeNewline(newOgrenci->name);
            removeNewline(newOgrenci->surName);
            newOgrenci->next = NULL;
            newOgrenci->prev = NULL;
            if(*head == NULL){
                *head = newOgrenci;
                tail = newOgrenci;
            } else {
                tail->next = newOgrenci;
                newOgrenci->prev = tail;
                tail = newOgrenci;
            }
        } else {
            printf("Uyarı: %s dosyasındaki satır okunamadı: %s\n", filename, line);
            free(newOgrenci);
        }
    }
    fclose(f);
}

OGRENCI *findOgrenciByNumber(OGRENCI *head, int number)
{
    OGRENCI *current = head;
    while(current != NULL){
        if(current->number == number){
            return current;
        }
        current = current->next;
    }
    return NULL;
}

OGRENCI* findOgrenciByName(OGRENCI* head, const char* name, const char* surname) {
    OGRENCI* current = head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0 && strcmp(current->surName, surname) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void insertOgrenci(OGRENCI **head, const char *ogrenciler_file)
{
    OGRENCI *newOgrenci = (OGRENCI *)malloc(sizeof(OGRENCI));
    OGRENCI *tmp;
    int tempNumber;
    int isEightDigits;

    if(newOgrenci == NULL){
        printf("Hata: Yeni öğrenci için bellek ayrılamadı.\n");
        return;
    }

    newOgrenci->libraryScore = 100; // Initial score
    newOgrenci->next = NULL;
    newOgrenci->prev = NULL;

    printf("Öğrencinin adını girin: ");
    fgets(newOgrenci->name, NAME_LEN, stdin);
    removeNewline(newOgrenci->name);

    printf("Öğrencinin soyadını girin: ");
    fgets(newOgrenci->surName, NAME_LEN, stdin);
    removeNewline(newOgrenci->surName);

    printf("Öğrenci numarasını girin (8 haneli ve benzersiz olmalı): ");
    if (scanf("%d", &tempNumber) != 1) {
        printf("Geçersiz numara girişi.\n");
        while(getchar() != '\n');
        free(newOgrenci);
        return;
    }
    getchar(); // Consume newline

    isEightDigits = (tempNumber >= 10000000 && tempNumber <= 99999999);
    if(!isEightDigits){
        printf("Hata: Öğrenci numarası 8 haneli olmalıdır.\n");
        free(newOgrenci);
        return;
    }
    if(findOgrenciByNumber(*head, tempNumber) != NULL){
        printf("Hata: %d numaralı öğrenci zaten mevcut. Benzersiz bir numara girin.\n", tempNumber);
        free(newOgrenci);
        return;
    }
    newOgrenci->number = tempNumber;

    if(*head == NULL){
        *head = newOgrenci;
    } else {
        tmp = *head;
        while(tmp->next != NULL){
            tmp = tmp->next;
        }
        tmp->next = newOgrenci;
        newOgrenci->prev = tmp;
    }
    saveOgrenciler(*head, ogrenciler_file);
    printf("Öğrenci eklendi: %s %s, Numara: %d [Puan: %d]\n", newOgrenci->name, newOgrenci->surName, newOgrenci->number, newOgrenci->libraryScore);
}

void deleteOgrenci(OGRENCI **head, const char *ogrenciler_file)
{
    OGRENCI *toDelete;
    int deleteNum;

    if(*head == NULL){
        printf("Öğrenci listesi boş.\n");
        return;
    }
    printf("Silmek istediğiniz öğrencinin numarasını girin: ");
     if (scanf("%d", &deleteNum) != 1) {
        printf("Geçersiz numara girişi.\n");
        while(getchar() != '\n');
        return;
    }
    getchar();

    toDelete = findOgrenciByNumber(*head, deleteNum);

    if(toDelete == NULL) {
        printf("%d numaralı öğrenci bulunamadı.\n", deleteNum);
        return;
    }

    // Adjust links
    if(toDelete->prev != NULL) {
        toDelete->prev->next = toDelete->next;
    } else { // Deleting head
        *head = toDelete->next;
    }
    if(toDelete->next != NULL) {
        toDelete->next->prev = toDelete->prev;
    }

    printf("Öğrenci %s %s (No: %d) silindi.\n", toDelete->name, toDelete->surName, toDelete->number);
    free(toDelete);
    saveOgrenciler(*head, ogrenciler_file);
}

void updateOgrenci(OGRENCI *head, const char *ogrenciler_file)
{
    OGRENCI *targetOgrenci;
    int number;
    char newName[NAME_LEN], newSurname[NAME_LEN];

    if(head == NULL){
        printf("Öğrenci listesi boş.\n");
        return;
    }
    printf("Güncellemek istediğiniz öğrencinin numarasını girin: ");
    if (scanf("%d", &number) != 1) {
        printf("Geçersiz numara girişi.\n");
        while(getchar() != '\n');
        return;
    }
    getchar();

    targetOgrenci = findOgrenciByNumber(head, number);

    if(targetOgrenci != NULL){
        printf("Mevcut ad: %s\n", targetOgrenci->name);
        printf("Yeni adı girin (değiştirmek istemiyorsanız boş bırakın): ");
        fgets(newName, NAME_LEN, stdin);
        removeNewline(newName);
        if(strlen(newName) > 0){
            strcpy(targetOgrenci->name, newName);
        }

        printf("Mevcut soyad: %s\n", targetOgrenci->surName);
        printf("Yeni soyadı girin (değiştirmek istemiyorsanız boş bırakın): ");
        fgets(newSurname, NAME_LEN, stdin);
        removeNewline(newSurname);
        if(strlen(newSurname) > 0){
            strcpy(targetOgrenci->surName, newSurname);
        }
        // Puan güncellemesi PDF'te belirtilmemiş, sadece otomatik ceza var.
        saveOgrenciler(head, ogrenciler_file);
        printf("Öğrenci No %d başarıyla güncellendi: %s %s, Puan: %d\n", targetOgrenci->number, targetOgrenci->name, targetOgrenci->surName, targetOgrenci->libraryScore);
    } else {
        printf("%d numaralı öğrenci bulunamadı.\n", number);
    }
}

void listOgrenciler(OGRENCI *head)
{
    OGRENCI *current = head;
    if(head == NULL){
        printf("Görüntülenecek öğrenci yok.\n");
        return;
    }
    printf("\n--- Öğrenci Listesi ---\n");
    while(current != NULL){
        printf("Numara: %d, Ad: %s, Soyad: %s, Puan: %d\n", current->number, current->name, current->surName, current->libraryScore);
        current = current->next;
    }
    printf("--------------------\n");
}

void listStudentInfoAndBorrowsByNumber(OGRENCI* ogrenciList, ODUNCKAYDI* oduncList) {
    int studentNum;
    OGRENCI* student;
    ODUNCKAYDI* k;
    int movementsFound = 0;

    printf("Bilgilerini görüntülemek istediğiniz öğrencinin numarasını girin: ");
    if (scanf("%d", &studentNum) != 1) {
        printf("Geçersiz giriş.\n"); while (getchar() != '\n'); return;
    }
    getchar();

    student = findOgrenciByNumber(ogrenciList, studentNum);
    if (student == NULL) {
        printf("Öğrenci %d bulunamadı.\n", studentNum);
        return;
    }
    printf("\n--- Öğrenci Bilgileri ---\n");
    printf("Numara: %d, Ad: %s %s, Puan: %d\n", student->number, student->name, student->surName, student->libraryScore);
    printf("Kitap Hareketleri:\n");

    k = oduncList;
    while (k != NULL) {
        if (k->OgrenciNumber == studentNum) {
            printf("  Etiket: %s, İşlem: %s, Tarih: %s\n", k->kitapEtiketNo, (k->islemTuru == 0 ? "Ödünç Alındı" : "Teslim Edildi"), k->tarih);
            movementsFound = 1;
        }
        k = k->next;
    }
    if (!movementsFound) printf("  Bu öğrenci için kitap hareketi bulunamadı.\n");
    printf("---------------------------\n");
}

void listStudentInfoAndBorrowsByName(OGRENCI* ogrenciList, ODUNCKAYDI* oduncList) {
    char name[NAME_LEN], surname[NAME_LEN];
    OGRENCI* student;
    ODUNCKAYDI* k;
    int movementsFound = 0;

    printf("Öğrencinin adını girin: ");
    fgets(name, NAME_LEN, stdin); removeNewline(name);
    printf("Öğrencinin soyadını girin: ");
    fgets(surname, NAME_LEN, stdin); removeNewline(surname);

    student = findOgrenciByName(ogrenciList, name, surname);
    if (student == NULL) {
        printf("Öğrenci %s %s bulunamadı.\n", name, surname);
        return;
    }

    printf("\n--- Öğrenci Bilgileri ---\n");
    printf("Numara: %d, Ad: %s %s, Puan: %d\n", student->number, student->name, student->surName, student->libraryScore);
    printf("Kitap Hareketleri:\n");

    k = oduncList;
    while (k != NULL) {
        if (k->OgrenciNumber == student->number) {
            printf("  Etiket: %s, İşlem: %s, Tarih: %s\n", k->kitapEtiketNo, (k->islemTuru == 0 ? "Ödünç Alındı" : "Teslim Edildi"), k->tarih);
            movementsFound = 1;
        }
        k = k->next;
    }
    if (!movementsFound) printf("  Bu öğrenci için kitap hareketi bulunamadı.\n");
    printf("---------------------------\n");
}


// --- Kitap and KitapCopy Functions ---
void freeKitapCopies(KITAPCOPY *head)
{
    KITAPCOPY *current = head;
    KITAPCOPY *nextCopy;
    while(current != NULL){
        nextCopy = current->next;
        free(current);
        current = nextCopy;
    }
}

void saveKitaplar(KITAP *head, const char *filename)
{
    KITAP *currentBook = head;
    FILE *f = fopen(filename, "w");
    if(f == NULL){
        printf("Hata: %s dosyası yazmak için açılamadı!\n", filename);
        return;
    }
    while(currentBook != NULL){
        fprintf(f, "%s,%s,%d\n", currentBook->name, currentBook->isbn, currentBook->copyCount);
        currentBook = currentBook->next;
    }
    fclose(f);
}

KITAP* findKitapByISBN(KITAP *head, const char *isbn)
{
    KITAP *current = head;
    while(current != NULL){
        if(strcmp(current->isbn, isbn) == 0){
            return current;
        }
        current = current->next;
    }
    return NULL;
}

KITAP* findKitapByName(KITAP* head, const char* name) {
    KITAP* current = head;
    while (current != NULL) {
        // Case-insensitive comparison might be better, but sticking to exact for now
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}


void loadKitaplar(KITAP **head, ODUNCKAYDI *oduncListHead, const char *filename)
{
    KITAP *newKitap, *lastKitapInList = NULL;
    char line[BOOK_NAME_LEN + ISBN_LEN + 10]; // Buffer for line
    FILE *f = fopen(filename, "r");

    *head = NULL; // Initialize

    if(f == NULL){
        // printf("%s dosyası bulunamadı. Kitap listesi boş başlatılıyor.\n", filename);
        return;
    }

    // Phase 1: Load books and create their copies with default "RAFTA" status
    while(fgets(line, sizeof(line), f)){
        char csv_bookName[BOOK_NAME_LEN];
        char csv_isbn[ISBN_LEN];
        int csv_copyCount;

        removeNewline(line);
        if(sscanf(line, "%[^,],%[^,],%d", csv_bookName, csv_isbn, &csv_copyCount) == 3){
            removeNewline(csv_bookName);
            removeNewline(csv_isbn);

            newKitap = (KITAP *)malloc(sizeof(KITAP));
            if(newKitap == NULL){
                printf("Hata: KITAP için bellek ayrılamadı.\n"); fclose(f);
                // Free already loaded books and their copies
                KITAP *tempK;
                while(*head != NULL) {
                    tempK = *head;
                    *head = (*head)->next;
                    freeKitapCopies(tempK->copies);
                    free(tempK);
                }
                return;
            }
            strcpy(newKitap->name, csv_bookName);
            strcpy(newKitap->isbn, csv_isbn);
            newKitap->copyCount = csv_copyCount;
            newKitap->copies = NULL;
            newKitap->next = NULL;

            KITAPCOPY *lastCopyNode = NULL;
            for(int i = 1; i <= newKitap->copyCount; i++){ // Corrected loop variable scope
                KITAPCOPY *newCopy = (KITAPCOPY *)malloc(sizeof(KITAPCOPY));
                if(newCopy == NULL){
                    printf("Hata: KITAPCOPY yüklenirken bellek ayrılamadı.\n");
                    freeKitapCopies(newKitap->copies);
                    free(newKitap);
                    fclose(f);
                     // Free already loaded books and their copies
                    KITAP *tempK;
                    while(*head != NULL) {
                        tempK = *head;
                        *head = (*head)->next;
                        freeKitapCopies(tempK->copies);
                        free(tempK);
                    }
                    return;
                }
                sprintf(newCopy->label, "%s_%d", newKitap->isbn, i);
                strcpy(newCopy->status, "RAFTA"); // Default status
                newCopy->next = NULL;

                if(newKitap->copies == NULL){
                    newKitap->copies = newCopy;
                } else {
                    lastCopyNode->next = newCopy;
                }
                lastCopyNode = newCopy;
            }

            if(*head == NULL){
                *head = newKitap;
            } else {
                lastKitapInList->next = newKitap;
            }
            lastKitapInList = newKitap;
        } else {
            printf("Uyarı: %s dosyasındaki satır okunamadı: %s\n", filename, line);
        }
    }
    fclose(f);

    // Phase 2: Update copy statuses based on oduncListHead
    KITAP *currentBook = *head;
    while(currentBook != NULL){
        KITAPCOPY *currentCopy = currentBook->copies;
        while(currentCopy != NULL){
            ODUNCKAYDI *latestActiveBorrow = NULL;
            ODUNCKAYDI *k = oduncListHead;
            while(k != NULL){
                if(strcmp(k->kitapEtiketNo, currentCopy->label) == 0 && k->islemTuru == 0){ // It's a borrow
                    // Check if this borrow is more recent
                    if(latestActiveBorrow == NULL || calculateDaysDifference(latestActiveBorrow->tarih, k->tarih) > 0){
                        // Verify this borrow (k) hasn't been returned by the same student
                        int wasReturned = 0;
                        ODUNCKAYDI *returnSearch = oduncListHead;
                        while(returnSearch != NULL && !wasReturned){ // Loop optimization
                            if(strcmp(returnSearch->kitapEtiketNo, k->kitapEtiketNo) == 0 &&
                               returnSearch->islemTuru == 1 &&
                               returnSearch->OgrenciNumber == k->OgrenciNumber &&
                               calculateDaysDifference(k->tarih, returnSearch->tarih) >= 0){
                                wasReturned = 1;
                            }
                            if (!wasReturned) returnSearch = returnSearch->next;
                        }
                        if(!wasReturned){
                            latestActiveBorrow = k;
                        }
                    }
                }
                k = k->next;
            }

            if(latestActiveBorrow != NULL){
                sprintf(currentCopy->status, "%d", latestActiveBorrow->OgrenciNumber);
            } else {
                strcpy(currentCopy->status, "RAFTA"); // Ensure it's RAFTA if no active borrow
            }
            currentCopy = currentCopy->next;
        }
        currentBook = currentBook->next;
    }
}


void addBook(KITAP **head, const char *kitaplar_file)
{
    KITAP *newKitap = (KITAP*) malloc(sizeof(KITAP));
    char name_input[BOOK_NAME_LEN]; // Corrected buffer size
    char isbn_input[ISBN_LEN];
    int adet;
    int isbnOk = 0;

    if(newKitap == NULL){
        printf("Hata: Yeni kitap için bellek ayrılamadı.\n");
        return;
    }

    printf("Kitap adını girin: ");
    fgets(name_input, BOOK_NAME_LEN, stdin); // Use correct buffer
    removeNewline(name_input);

    while(!isbnOk){
        printf("Kitap ISBN'sini girin (%d karakter): ", ISBN_LEN -1);
        fgets(isbn_input, ISBN_LEN, stdin);
        removeNewline(isbn_input);
        if(strlen(isbn_input) == ISBN_LEN - 1){
            if(findKitapByISBN(*head, isbn_input) == NULL){
                isbnOk=1;
            } else {
                printf("%s ISBN'li bir kitap zaten mevcut. Yinelenen ISBN eklenemez.\n", isbn_input);
                free(newKitap); // Free allocated memory before returning
                return;
            }
        } else {
            printf("ISBN tam olarak %d karakter uzunluğunda olmalıdır.\n", ISBN_LEN - 1);
        }
    }

    printf("Kopya sayısını girin: ");
    if (scanf("%d", &adet) != 1 || adet < 0) {
        printf("Geçersiz kopya sayısı.\n");
        while(getchar() != '\n');
        free(newKitap);
        return;
    }
    getchar(); // Consume newline

    strcpy(newKitap->name, name_input);
    strcpy(newKitap->isbn, isbn_input);
    newKitap->copyCount = adet;
    newKitap->copies = NULL;
    newKitap->next = NULL;

    KITAPCOPY *lastCopy = NULL;
    for(int i = 1; i <= adet; i++) { // Use for loop
        KITAPCOPY *copy = (KITAPCOPY*) malloc(sizeof(KITAPCOPY));
        if (copy == NULL) {
            printf("Hata: Kitap kopyası için bellek ayrılamadı.\n");
            freeKitapCopies(newKitap->copies); // Free already created copies for this book
            free(newKitap);
            return;
        }
        sprintf(copy->label, "%s_%d", newKitap->isbn, i);
        strcpy(copy->status, "RAFTA");
        copy->next = NULL;

        if(newKitap->copies == NULL){
            newKitap->copies = copy;
        } else {
            lastCopy->next = copy;
        }
        lastCopy = copy;
    }

    if(*head == NULL){
        *head = newKitap;
    } else {
        KITAP *tmp = *head;
        while(tmp->next != NULL){
            tmp = tmp->next;
        }
        tmp->next = newKitap;
    }

    saveKitaplar(*head, kitaplar_file);
    printf("Kitap eklendi: %s | ISBN: %s | Kopyalar: %d\n", newKitap->name, newKitap->isbn, newKitap->copyCount);
}

void deleteKitap(KITAP **head, KITAPYAZAR_REL **kyRelations, int *kyCount, const char *kitaplar_file, const char *kitapyazar_file)
{
    KITAP *current, *prev;
    char isbnToDelete[ISBN_LEN];
    int canDelete = 1;

    if(*head == NULL){
        printf("Kitap listesi boş.\n");
        return;
    }
    printf("Silinecek kitabın ISBN'sini girin: ");
    fgets(isbnToDelete, ISBN_LEN, stdin);
    removeNewline(isbnToDelete);

    current = *head;
    prev = NULL;

    // Find the book node
    while(current != NULL && strcmp(current->isbn, isbnToDelete) != 0){
        prev = current;
        current = current->next;
    }

    if(current == NULL){ // Not found
        printf("%s ISBN'li kitap bulunamadı.\n", isbnToDelete);
        return;
    }

    // Found: current is the node to delete
    KITAPCOPY *copyCheck = current->copies;
    while(copyCheck != NULL && canDelete){ // Optimized loop condition
        if(strcmp(copyCheck->status, "RAFTA") != 0){
            printf("Kitap %s (ISBN: %s) silinemez. %s kopyası şu anda %s numaralı öğrenci tarafından ödünç alınmış.\n",
                current->name, current->isbn, copyCheck->label, copyCheck->status);
            canDelete = 0;
        }
        if (canDelete) copyCheck = copyCheck->next;
    }

    if(canDelete){
        if(prev == NULL){ // Deleting head
            *head = current->next;
        } else {
            prev->next = current->next;
        }
        printf("Kitap %s (ISBN: %s) ve kopyaları silindi.\n", current->name, current->isbn);
        freeKitapCopies(current->copies);
        free(current);
        saveKitaplar(*head, kitaplar_file);

        // Update KitapYazar relations: remove relations for the deleted ISBN
        KITAPYAZAR_REL *newRelations = NULL;
        int newCount = 0;
        for(int i = 0; i < *kyCount; i++){
            if(strcmp((*kyRelations)[i].isbn, isbnToDelete) != 0){
                KITAPYAZAR_REL *temp_realloc = (KITAPYAZAR_REL *)realloc(newRelations, (newCount + 1) * sizeof(KITAPYAZAR_REL));
                if(temp_realloc == NULL){
                    printf("Hata: KitapYazar ilişkileri silinirken bellek yeniden tahsis edilemedi.\n");
                    // To prevent data loss, keep old relations if realloc failed mid-way
                    if (newRelations) free(newRelations); // Free partially built new array
                    // The original *kyRelations is still intact. We might choose not to save.
                    return; // Exit to prevent further issues
                }
                newRelations = temp_realloc;
                newRelations[newCount] = (*kyRelations)[i];
                newCount++;
            }
        }
        free(*kyRelations); // Free the old array
        *kyRelations = newRelations;
        *kyCount = newCount;
        saveKitapYazarRelations(*kyRelations, *kyCount, kitapyazar_file);
        printf("Silinen %s ISBN'li kitap için KitapYazar ilişkileri kaldırıldı.\n", isbnToDelete);
    }
}

void updateKitap(KITAP *head, const char *kitaplar_file)
{
    char isbnToUpdate[ISBN_LEN];
    KITAP *book;
    char newName[BOOK_NAME_LEN];
    int newTotalCopiesInput;

    if(head == NULL){
        printf("Kitap listesi boş.\n");
        return;
    }

    printf("Güncellenecek kitabın ISBN'sini girin: ");
    fgets(isbnToUpdate, ISBN_LEN, stdin);
    removeNewline(isbnToUpdate);

    book = findKitapByISBN(head, isbnToUpdate);
    if(book == NULL){
        printf("%s ISBN'li kitap bulunamadı.\n", isbnToUpdate);
        return;
    }

    printf("Kitap güncelleniyor: %s (ISBN: %s)\n", book->name, book->isbn);
    printf("Mevcut ad: %s\n", book->name);
    printf("Yeni adı girin (değiştirmek istemiyorsanız boş bırakın): ");
    fgets(newName, BOOK_NAME_LEN, stdin);
    removeNewline(newName);
    if(strlen(newName) > 0){
        strcpy(book->name, newName);
    }

    printf("Mevcut toplam kopya sayısı: %d\n", book->copyCount);
    printf("Yeni toplam kopya sayısını girin (atlamak için 0, mevcut ödünç alınmış kopyalardan >= olmalı): ");
    if (scanf("%d", &newTotalCopiesInput) != 1){
        printf("Kopya sayısı için geçersiz giriş.\n");
        while(getchar()!='\n'); // Clear buffer
        saveKitaplar(head, kitaplar_file); // Save any name changes
        return;
    }
    getchar(); // Consume newline

    if(newTotalCopiesInput >= 0){ // Allow setting to 0 if no copies borrowed
        int nonRaftaCount = 0;
        KITAPCOPY *kc = book->copies;
        while(kc != NULL){
            if(strcmp(kc->status, "RAFTA") != 0) nonRaftaCount++;
            kc = kc->next;
        }

        if(newTotalCopiesInput < nonRaftaCount){
            printf("Yeni kopya sayısı (%d), mevcut ödünç alınmış kopya sayısından (%d) az olamaz.\n", newTotalCopiesInput, nonRaftaCount);
        } else {
            if(newTotalCopiesInput > book->copyCount){ // Add copies
                int copiesToAdd = newTotalCopiesInput - book->copyCount;
                KITAPCOPY *lastCopy = book->copies;
                if(lastCopy != NULL) while(lastCopy->next != NULL) lastCopy = lastCopy->next;

                int currentMaxCopyNum = 0;
                kc = book->copies;
                while(kc != NULL){
                    int num_suffix;
                    if(sscanf(kc->label, "%*[^_]_%d", &num_suffix) == 1){ // Extract number after '_'
                        if(num_suffix > currentMaxCopyNum) currentMaxCopyNum = num_suffix;
                    }
                    kc = kc->next;
                }

                int successfullyAdded = 0;
                for(int i = 0; i < copiesToAdd; i++){
                    currentMaxCopyNum++;
                    KITAPCOPY *newCopyNode = (KITAPCOPY *)malloc(sizeof(KITAPCOPY));
                    if (newCopyNode) {
                        sprintf(newCopyNode->label, "%s_%d", book->isbn, currentMaxCopyNum);
                        strcpy(newCopyNode->status, "RAFTA");
                        newCopyNode->next = NULL;
                        if (book->copies == NULL) book->copies = newCopyNode; // If list was empty
                        else if (lastCopy == NULL) book->copies = newCopyNode; // Should be covered by book->copies == NULL
                        else lastCopy->next = newCopyNode;
                        lastCopy = newCopyNode;
                        successfullyAdded++;
                    } else {
                        printf("Hata: %d kopya eklendikten sonra yeni kopya için bellek ayrılamadı.\n", successfullyAdded);
                        i = copiesToAdd; // Terminate loop
                    }
                }
                if (successfullyAdded > 0) printf("%d kopya eklendi.\n", successfullyAdded);

            } else if(newTotalCopiesInput < book->copyCount){ // Remove copies
                int copiesToRemove = book->copyCount - newTotalCopiesInput;
                int removedCount = 0;
                KITAPCOPY *currentCopy = book->copies;
                KITAPCOPY *prevCopy = NULL;

                while(currentCopy != NULL && removedCount < copiesToRemove){
                    if(strcmp(currentCopy->status, "RAFTA") == 0){
                        KITAPCOPY *toDelete = currentCopy;
                        if(prevCopy == NULL){ // Removing head of copies list
                            book->copies = currentCopy->next;
                        } else {
                            prevCopy->next = currentCopy->next;
                        }
                        currentCopy = currentCopy->next; // Move current before freeing
                        printf("Kopya kaldırıldı: %s\n", toDelete->label);
                        free(toDelete);
                        removedCount++;
                        // prevCopy remains the same if head was removed, or points to the node before current
                        // if current was not head. If current becomes NULL, prevCopy is the new tail.
                    } else {
                        prevCopy = currentCopy;
                        currentCopy = currentCopy->next;
                    }
                }
                if(removedCount < copiesToRemove){
                    printf("%d RAFTA kopya kaldırılamadı. Sadece %d kopya kaldırıldı.\n", copiesToRemove, removedCount);
                } else if (removedCount > 0) {
                    printf("%d kopya kaldırıldı.\n", removedCount);
                }
            }
            book->copyCount = newTotalCopiesInput; // Set the final copy count
        }
    }
    saveKitaplar(head, kitaplar_file);
    printf("%s ISBN'li kitap güncellendi.\n", book->isbn);
}

void listAllKitaplarWithCopies(KITAP *head){
    KITAP *currentBook = head;

    if(head == NULL){
        printf("Görüntülenecek kitap yok.\n");
        return;
    }
    printf("\n--- Kitap Listesi ---\n");
    while(currentBook != NULL){
        printf("Kitap: %s (ISBN: %s, Sistemdeki Toplam Kopya: %d)\n", currentBook->name, currentBook->isbn, currentBook->copyCount);
        KITAPCOPY *currentCopy = currentBook->copies;
        if(currentCopy == NULL && currentBook->copyCount > 0){
            printf("  Kopya listesi boş ancak sayı %d (yükleme/güncelleme sırasında olası sorun).\n", currentBook->copyCount);
        } else if(currentCopy == NULL && currentBook->copyCount == 0){
            printf("  Bu kitap için kayıtlı kopya yok.\n");
        } else {
            printf("  Bireysel Kopyalar:\n");
            while(currentCopy != NULL){
                printf("    Etiket: %s, Durum: %s\n", currentCopy->label, currentCopy->status);
                currentCopy = currentCopy->next;
            }
        }
        currentBook = currentBook->next;
    }
    printf("-----------------\n");
}

void listKitapInfoByName(KITAP *kitapList) {
    char name_input[BOOK_NAME_LEN];
    KITAP *kitap;

    printf("Bilgilerini görüntülemek istediğiniz kitabın adını girin: ");
    fgets(name_input, BOOK_NAME_LEN, stdin);
    removeNewline(name_input);

    kitap = findKitapByName(kitapList, name_input);

    if (kitap == NULL) {
        printf("'%s' adlı kitap bulunamadı.\n", name_input);
        return;
    }

    printf("\n--- Kitap Bilgileri ---\n");
    printf("Kitap: %s (ISBN: %s, Sistemdeki Toplam Kopya: %d)\n", kitap->name, kitap->isbn, kitap->copyCount);
    KITAPCOPY *currentCopy = kitap->copies;
    if (currentCopy == NULL && kitap->copyCount > 0) {
        printf("  Kopya listesi boş ancak sayı %d.\n", kitap->copyCount);
    } else if (currentCopy == NULL && kitap->copyCount == 0) {
        printf("  Bu kitap için kayıtlı kopya yok.\n");
    } else {
        printf("  Bireysel Kopyalar:\n");
        while (currentCopy != NULL) {
            printf("    Etiket: %s, Durum: %s\n", currentCopy->label, currentCopy->status);
            currentCopy = currentCopy->next;
        }
    }
    printf("-----------------\n");
}


// --- KitapYazarRel Functions ---
void saveKitapYazarRelations(KITAPYAZAR_REL *relations, int count, const char *filename)
{
    FILE *f = fopen(filename, "w");
    if(f == NULL){
        printf("Hata: %s dosyası yazmak için açılamadı!\n", filename);
        return;
    }
    for(int i = 0; i < count; i++){
        fprintf(f, "%s,%d\n", relations[i].isbn, relations[i].yazarID);
    }
    fclose(f);
}

void loadKitapYazarRelations(KITAPYAZAR_REL **relations, int *count, const char *filename)
{
    char line[ISBN_LEN + NAME_LEN + 5]; // Buffer size
    FILE *f = fopen(filename, "r");

    *relations = NULL;
    *count = 0;

    if (f == NULL){
        // printf("%s dosyası bulunamadı. Kitap-Yazar ilişkileri boş başlatılıyor.\n", filename);
        return;
    }

    while(fgets(line, sizeof(line), f)){
        KITAPYAZAR_REL tempRel;
        removeNewline(line);
        if(sscanf(line, "%[^,],%d", tempRel.isbn, &tempRel.yazarID) == 2){
            removeNewline(tempRel.isbn);
            KITAPYAZAR_REL *temp_ptr = (KITAPYAZAR_REL *)realloc(*relations, (*count + 1) * sizeof(KITAPYAZAR_REL));
            if (temp_ptr == NULL){
                printf("Hata: KitapYazar ilişkileri için bellek yeniden tahsis edilemedi.\n");
                free(*relations); // Free what was allocated so far
                *relations = NULL;
                *count = 0;
                fclose(f);
                return;
            }
            *relations = temp_ptr;
            (*relations)[*count] = tempRel;
            (*count)++;
        } else {
            printf("Uyarı: %s dosyasındaki satır okunamadı: %s\n", filename, line);
        }
    }
    fclose(f);
}

void addKitapYazarRelation(KITAPYAZAR_REL **relations, int *count, KITAP *kitapList, YAZAR *yazarList, const char *kitapyazar_file)
{
    char isbn_input[ISBN_LEN];
    int yazarId_input;
    KITAP *kitap;
    YAZAR *yazar;

    printf("İlişkilendirilecek Kitap ISBN'sini girin: ");
    fgets(isbn_input, ISBN_LEN, stdin);
    removeNewline(isbn_input);

    kitap = findKitapByISBN(kitapList, isbn_input);
    if(kitap == NULL){
        printf("%s ISBN'li kitap bulunamadı.\n", isbn_input);
        return;
    }

    printf("İlişkilendirilecek Yazar ID'sini girin: ");
    if(scanf("%d", &yazarId_input) != 1){
        printf("Geçersiz Yazar ID girişi.\n");
        while(getchar()!='\n');
        return;
    }
    getchar(); // Consume newline

    yazar = findYazarById(yazarList, yazarId_input);
    if(yazar == NULL){
        printf("%d ID'li yazar bulunamadı.\n", yazarId_input);
        return;
    }
    if (yazar->id == -1) { // Check if author was marked as deleted
        printf("%d ID'li yazar sistemden silinmiş, ilişki kurulamaz.\n", yazarId_input);
        return;
    }


    for(int i=0; i < *count; i++){
        if(strcmp((*relations)[i].isbn, isbn_input) == 0 && (*relations)[i].yazarID == yazarId_input){
            printf("Bu kitap-yazar ilişkisi zaten mevcut.\n");
            return;
        }
    }

    KITAPYAZAR_REL *temp_ptr = (KITAPYAZAR_REL *)realloc(*relations, (*count + 1) * sizeof(KITAPYAZAR_REL));
    if (temp_ptr == NULL){
        printf("Hata: Yeni KitapYazar ilişkisi için bellek yeniden tahsis edilemedi.\n");
        return; // Keep existing relations intact
    }
    *relations = temp_ptr;

    strcpy((*relations)[*count].isbn, isbn_input);
    (*relations)[*count].yazarID = yazarId_input;
    (*count)++;

    saveKitapYazarRelations(*relations, *count, kitapyazar_file);
    printf("İlişki eklendi: Kitap ISBN %s - Yazar ID %d\n", isbn_input, yazarId_input);
}

void listKitapYazarRelations(KITAPYAZAR_REL *relations, int count)
{
    if (count == 0){
        printf("Görüntülenecek Kitap-Yazar ilişkisi yok.\n");
        return;
    }
    printf("\n--- Kitap-Yazar İlişkileri ---\n");
    for(int i = 0; i < count; i++){
         // Only display if yazarID is not -1 (not deleted)
        if (relations[i].yazarID != -1) {
            printf("Kitap ISBN: %s, Yazar ID: %d\n", relations[i].isbn, relations[i].yazarID);
        } else {
            printf("Kitap ISBN: %s, Yazar ID: (Silinmiş Yazar - Eski ID: %d)\n", relations[i].isbn, relations[i].yazarID);
        }
    }
    printf("-----------------------------\n");
}

void updateBookAuthors(KITAP *kitapList, YAZAR *yazarList, KITAPYAZAR_REL **relations, int *count, const char *kitapyazar_file) {
    char isbn_input[ISBN_LEN];
    KITAP *book;
    int choice, yazarId_input;
    int i, k;
    int foundRel;

    printf("Yazarlarını güncellemek istediğiniz kitabın ISBN'sini girin: ");
    fgets(isbn_input, ISBN_LEN, stdin);
    removeNewline(isbn_input);

    book = findKitapByISBN(kitapList, isbn_input);
    if (book == NULL) {
        printf("%s ISBN'li kitap bulunamadı.\n", isbn_input);
        return;
    }

    printf("Kitap: %s (ISBN: %s)\n", book->name, book->isbn);
    printf("Mevcut Yazarlar:\n");
    foundRel = 0;
    for (i = 0; i < *count; i++) {
        if (strcmp((*relations)[i].isbn, isbn_input) == 0 && (*relations)[i].yazarID != -1) {
            YAZAR *author = findYazarById(yazarList, (*relations)[i].yazarID);
            if (author) {
                printf("  ID: %d, Ad: %s %s\n", author->id, author->name, author->surName);
                foundRel = 1;
            }
        }
    }
    if (!foundRel) {
        printf("  Bu kitap için kayıtlı yazar yok.\n");
    }

    printf("\nİşlem seçin:\n1. Yazar Ekle\n2. Yazar Sil\n0. İptal\nSeçiminiz: ");
    if (scanf("%d", &choice) != 1) {
        printf("Geçersiz seçim.\n");
        while(getchar() != '\n'); return;
    }
    getchar();

    if (choice == 1) { // Add author
        printf("Eklenecek Yazar ID'sini girin: ");
        if (scanf("%d", &yazarId_input) != 1) {
            printf("Geçersiz Yazar ID.\n"); while(getchar() != '\n'); return;
        }
        getchar();
        YAZAR *author_to_add = findYazarById(yazarList, yazarId_input);
        if (author_to_add == NULL || author_to_add->id == -1) {
            printf("%d ID'li yazar bulunamadı veya silinmiş.\n", yazarId_input);
            return;
        }
        // Check if relation already exists
        for (i = 0; i < *count; i++) {
            if (strcmp((*relations)[i].isbn, isbn_input) == 0 && (*relations)[i].yazarID == yazarId_input) {
                printf("Bu yazar zaten bu kitaba atanmış.\n");
                return;
            }
        }
        // Add new relation
        KITAPYAZAR_REL *temp_ptr = realloc(*relations, (*count + 1) * sizeof(KITAPYAZAR_REL));
        if (!temp_ptr) {
            printf("Hata: Bellek ayrılamadı.\n"); return;
        }
        *relations = temp_ptr;
        strcpy((*relations)[*count].isbn, isbn_input);
        (*relations)[*count].yazarID = yazarId_input;
        (*count)++;
        printf("Yazar ID %d, %s ISBN'li kitaba eklendi.\n", yazarId_input, isbn_input);

    } else if (choice == 2) { // Remove author
        if (!foundRel) {
             printf("Bu kitaptan silinecek yazar yok.\n"); return;
        }
        printf("Kaldırılacak Yazar ID'sini girin: ");
         if (scanf("%d", &yazarId_input) != 1) {
            printf("Geçersiz Yazar ID.\n"); while(getchar() != '\n'); return;
        }
        getchar();

        KITAPYAZAR_REL *new_rels = NULL;
        int new_rel_count = 0;
        int removed = 0;
        for (i = 0; i < *count; i++) {
            if (strcmp((*relations)[i].isbn, isbn_input) == 0 && (*relations)[i].yazarID == yazarId_input) {
                removed = 1; // Mark for removal, don't copy
            } else {
                KITAPYAZAR_REL *temp_ptr = realloc(new_rels, (new_rel_count + 1) * sizeof(KITAPYAZAR_REL));
                if (!temp_ptr) {
                    printf("Hata: Bellek ayrılamadı.\n"); free(new_rels); return;
                }
                new_rels = temp_ptr;
                new_rels[new_rel_count++] = (*relations)[i];
            }
        }
        if (removed) {
            free(*relations);
            *relations = new_rels;
            *count = new_rel_count;
            printf("Yazar ID %d, %s ISBN'li kitaptan kaldırıldı.\n", yazarId_input, isbn_input);
        } else {
            printf("Yazar ID %d, bu kitapla ilişkili bulunamadı.\n", yazarId_input);
            free(new_rels); // Free if nothing was changed to new_rels
        }
    } else if (choice == 0) {
        printf("İşlem iptal edildi.\n");
    } else {
        printf("Geçersiz seçim.\n");
    }
    if (choice == 1 || (choice == 2 && foundRel)) {
         saveKitapYazarRelations(*relations, *count, kitapyazar_file);
    }
}


// --- OduncKaydi Functions ---
void freeOduncList(ODUNCKAYDI *head)
{
    ODUNCKAYDI *current = head;
    ODUNCKAYDI *next;
    while(current != NULL){
        next = current->next;
        free(current);
        current = next;
    }
}

void saveOduncKayitlari(ODUNCKAYDI *head, const char *filename)
{
    ODUNCKAYDI *current = head;
    FILE *f = fopen(filename, "w");
    if(f == NULL){
        printf("Hata: %s dosyası yazmak için açılamadı!\n", filename);
        return;
    }
    while(current != NULL){
        fprintf(f, "%d,%s,%d,%s\n", current->OgrenciNumber, current->kitapEtiketNo, current->islemTuru, current->tarih);
        current = current->next;
    }
    fclose(f);
}

void loadOduncKayitlari(ODUNCKAYDI **head, const char *filename)
{
    char line[100];
    ODUNCKAYDI *tail = NULL;
    FILE *f = fopen(filename, "r");

    *head = NULL; // Initialize

    if(f == NULL){
        // printf("%s dosyası bulunamadı. Ödünç kayıtları boş başlatılıyor.\n", filename);
        return;
    }

    while(fgets(line, sizeof(line), f)){
        ODUNCKAYDI *newKayit = (ODUNCKAYDI *)malloc(sizeof(ODUNCKAYDI));
        if(newKayit == NULL){
            printf("Hata: OduncKaydi için bellek ayrılamadı.\n"); fclose(f);
            freeOduncList(*head); *head = NULL; // Free partially loaded list
            return;
        }
        removeNewline(line);
        if(sscanf(line, "%d,%[^,],%d,%s", &newKayit->OgrenciNumber, newKayit->kitapEtiketNo, &newKayit->islemTuru, newKayit->tarih) == 4){
            removeNewline(newKayit->kitapEtiketNo);
            removeNewline(newKayit->tarih);
            newKayit->next = NULL;
            if(*head == NULL){
                *head = newKayit;
            } else {
                tail->next = newKayit;
            }
            tail = newKayit;
        } else {
            printf("Uyarı: %s dosyasındaki satır okunamadı: %s\n", filename, line);
            free(newKayit);
        }
    }
    fclose(f);
}

void borrowKitap(OGRENCI *ogrenciList, KITAP *kitapList, ODUNCKAYDI **oduncListHead, const char *odunctakip_file, const char *ogrenciler_file)
{
    int studentNum;
    char isbnToBorrow[ISBN_LEN];
    KITAPCOPY *selectedCopy = NULL;
    OGRENCI *student;
    KITAP *book;

    printf("Öğrenci numarasını girin: ");
    if(scanf("%d", &studentNum) != 1){
        printf("Geçersiz öğrenci numarası girişi.\n");
        while(getchar()!='\n');
        return;
    }
    getchar(); // Consume newline

    student = findOgrenciByNumber(ogrenciList, studentNum);
    if(student == NULL){
        printf("%d numaralı öğrenci bulunamadı.\n", studentNum);
        return;
    }
    if(student->libraryScore <= 0){ // PDF says "negatif", this covers 0 too.
        printf("Öğrenci %d, kütüphane puanı %d olduğu için kitap ödünç alamaz.\n", studentNum, student->libraryScore);
        return;
    }

    printf("Ödünç alınacak kitabın ISBN'sini girin: ");
    fgets(isbnToBorrow, ISBN_LEN, stdin);
    removeNewline(isbnToBorrow);

    book = findKitapByISBN(kitapList, isbnToBorrow);
    if(book == NULL){
        printf("%s ISBN'li kitap bulunamadı.\n", isbnToBorrow);
        return;
    }

    KITAPCOPY *copy = book->copies;
    // Find first available copy
    while(copy != NULL && selectedCopy == NULL){ // Loop until copy found or end of list
        if(strcmp(copy->status, "RAFTA") == 0){
            selectedCopy = copy;
        }
        if (selectedCopy == NULL) copy = copy->next; // Advance only if not selected
    }


    if(selectedCopy == NULL){
        printf("%s (ISBN: %s) kitabının rafta uygun kopyası bulunmamaktadır.\n", book->name, book->isbn);
        return;
    }

    // Update copy status in memory
    sprintf(selectedCopy->status, "%d", studentNum);

    ODUNCKAYDI *newKayit = (ODUNCKAYDI *)malloc(sizeof(ODUNCKAYDI));
    if(newKayit == NULL){
        printf("Hata: Ödünç kaydı için bellek ayrılamadı.\n");
        strcpy(selectedCopy->status, "RAFTA"); // Revert status if malloc fails
        return;
    }
    newKayit->OgrenciNumber = studentNum;
    strcpy(newKayit->kitapEtiketNo, selectedCopy->label);
    newKayit->islemTuru = 0; // 0 for borrow
    getCurrentDate(newKayit->tarih);
    newKayit->next = NULL;

    // Add to oduncList
    if(*oduncListHead == NULL){
        *oduncListHead = newKayit;
    } else {
        ODUNCKAYDI *tmp = *oduncListHead;
        while(tmp->next != NULL) tmp = tmp->next;
        tmp->next = newKayit;
    }

    saveOduncKayitlari(*oduncListHead, odunctakip_file);
    // Student score is not changed on borrow, only on late return.
    // saveOgrenciler(ogrenciList, ogrenciler_file); // Not needed here

    printf("Kitap kopyası %s (ISBN: %s), %d numaralı öğrenci tarafından %s tarihinde ödünç alındı.\n", selectedCopy->label, book->isbn, studentNum, newKayit->tarih);
}

void returnKitap(OGRENCI *ogrenciList, KITAP *kitapList, ODUNCKAYDI **oduncListHead, const char *odunctakip_file, const char *ogrenciler_file)
{
    int studentNum;
    char labelToReturn[ISBN_LEN + 5];
    OGRENCI *student;
    KITAPCOPY *returnedCopy = NULL; // Initialize
    KITAP *currentBook = kitapList;
    int copyFoundAndBorrowedByStudent = 0;
    int stopSearch = 0; // Flag to stop searching loops

    printf("Kitabı iade eden öğrencinin numarasını girin: ");
    if(scanf("%d", &studentNum) != 1){
        printf("Geçersiz öğrenci numarası girişi.\n");
        while(getchar()!='\n');
        return;
    }
    getchar();

    student = findOgrenciByNumber(ogrenciList, studentNum);
    if(student == NULL){
        printf("%d numaralı öğrenci bulunamadı.\n", studentNum);
        return;
    }

    printf("İade edilen kitap kopyasının etiketini girin (örn: ISBN_1): ");
    fgets(labelToReturn, sizeof(labelToReturn), stdin);
    removeNewline(labelToReturn);

    // Find the copy and verify it's borrowed by this student
    while(currentBook != NULL && !stopSearch){
        KITAPCOPY *copy = currentBook->copies;
        while(copy != NULL && !stopSearch){
            if(strcmp(copy->label, labelToReturn) == 0){
                char studentNumStr[STATUS_LEN];
                sprintf(studentNumStr, "%d", studentNum);
                if(strcmp(copy->status, studentNumStr) == 0){
                    returnedCopy = copy;
                    copyFoundAndBorrowedByStudent = 1;
                    stopSearch = 1; // Found, stop all loops
                } else {
                    printf("%s etiketli kitap kopyası bulundu, ancak %d numaralı öğrenci tarafından ödünç alınmamış. Mevcut durum: %s\n", labelToReturn, studentNum, copy->status);
                    return; // Exit function as per original logic
                }
            }
            if(!stopSearch) copy = copy->next;
        }
        if(!stopSearch) currentBook = currentBook->next;
    }

    if(!copyFoundAndBorrowedByStudent || returnedCopy == NULL){ // Check both
        printf("%s etiketli kitap kopyası bulunamadı veya %d numaralı öğrenci tarafından aktif olarak ödünç alınmamış.\n", labelToReturn, studentNum);
        return;
    }

    // Find the latest borrow record for THIS student and THIS copy label
    ODUNCKAYDI *borrowEntry = NULL;
    ODUNCKAYDI *k = *oduncListHead;
    ODUNCKAYDI *latestBorrowForThisCopyByStudent = NULL;

    while(k != NULL){
        if(k->OgrenciNumber == studentNum && strcmp(k->kitapEtiketNo, labelToReturn) == 0 && k->islemTuru == 0){
            if(latestBorrowForThisCopyByStudent == NULL || calculateDaysDifference(latestBorrowForThisCopyByStudent->tarih, k->tarih) > 0){
                latestBorrowForThisCopyByStudent = k;
            }
        }
        k = k->next;
    }
    borrowEntry = latestBorrowForThisCopyByStudent;

    if(borrowEntry == NULL){
        printf("Kritik hata: Kopya durumu %d numaralı öğrenci tarafından ödünç alındığını gösteriyor, ancak OduncTakip.csv'de %s için aktif ödünç kaydı bulunamadı.\n", studentNum, labelToReturn);
        // Consider reverting status to RAFTA as a safety measure, but this indicates data inconsistency.
        // strcpy(returnedCopy->status, "RAFTA");
        return;
    }

    char currentDateStr[DATE_LEN];
    getCurrentDate(currentDateStr);
    int daysDiff = calculateDaysDifference(borrowEntry->tarih, currentDateStr);
    printf("Kitap ödünç alınma tarihi: %s, İade tarihi: %s. Fark: %d gün.\n", borrowEntry->tarih, currentDateStr, daysDiff);

    if(daysDiff > 15){
        student->libraryScore -= 10;
        printf("Kitap geç iade edildi (%d gün). -10 ceza puanı uygulandı. Öğrenci %d için yeni puan: %d\n", daysDiff, studentNum, student->libraryScore);
        saveOgrenciler(ogrenciList, ogrenciler_file); // Save updated student score
    }

    strcpy(returnedCopy->status, "RAFTA"); // Update copy status in memory

    ODUNCKAYDI *newKayit = (ODUNCKAYDI *) malloc(sizeof(ODUNCKAYDI));
    if(newKayit == NULL){
        printf("Hata: İade kaydı için bellek ayrılamadı.\n");
        sprintf(returnedCopy->status, "%d", studentNum); // Revert status
        if (daysDiff > 15) student->libraryScore +=10; // Revert score penalty if applicable
        // saveOgrenciler might be needed if score was reverted
        return;
    }
    newKayit->OgrenciNumber = studentNum;
    strcpy(newKayit->kitapEtiketNo, returnedCopy->label);
    newKayit->islemTuru = 1; // 1 for return
    strcpy(newKayit->tarih, currentDateStr);
    newKayit->next = NULL;

    // Add to oduncList
    if(*oduncListHead == NULL){
        *oduncListHead = newKayit;
    } else {
        ODUNCKAYDI *tmp = *oduncListHead;
        while(tmp->next != NULL) tmp = tmp->next;
        tmp->next = newKayit;
    }

    saveOduncKayitlari(*oduncListHead, odunctakip_file);
    printf("Kitap kopyası %s, %d numaralı öğrenci tarafından %s tarihinde iade edildi.\n", returnedCopy->label, studentNum, currentDateStr);
}


// --- Listing/Reporting Functions ---
void listStudentsWhoHaventReturnBooks(KITAP* kitapList, OGRENCI* ogrenciList)
{
    int foundAny = 0;
    KITAP* b = kitapList;
    printf("\n--- Kitap Teslim Etmemiş Öğrenciler ---\n");

    while(b != NULL){
        KITAPCOPY* c = b->copies;
        while(c != NULL){
            if(strcmp(c->status, "RAFTA") != 0){ // Status is a student number
                int studentNumFromStatus = atoi(c->status);
                OGRENCI* o = findOgrenciByNumber(ogrenciList, studentNumFromStatus);
                if(o != NULL){
                    printf("Öğrenci: %s %s (No: %d) kitap kopyasına sahip: %s (Kitap: %s)\n",
                        o->name, o->surName, o->number, c->label, b->name);
                    foundAny = 1;
                } else {
                    printf("Kitap kopyası: %s (Kitap: %s) %s numaralı öğrenci tarafından ödünç alınmış olarak listeleniyor, ancak öğrenci bulunamadı.\n",
                        c->label, b->name, c->status);
                     // This indicates a data inconsistency, possibly a deleted student.
                }
            }
            c = c->next;
        }
        b = b->next;
    }
    if(!foundAny) printf("Şu anda herhangi bir öğrenci tarafından ödünç alınmış olarak listelenen kitap yok.\n");
    printf("-------------------------------------\n");
}

void listPenalizedStudents(OGRENCI *head)
{
    OGRENCI *current = head;
    int found = 0;
    printf("\n--- Cezalı Öğrenciler (Puan < 100) ---\n");

    while (current != NULL){
        if(current->libraryScore < 100){
            printf("Numara: %d, Ad: %s %s, Puan: %d\n", current->number, current->name, current->surName, current->libraryScore);
            found = 1;
        }
        current = current->next;
    }
    if (!found) printf("Şu anda hiçbir öğrencinin puanı 100'ün altında değil.\n");
    printf("----------------------------------------\n");
}

void listBooksOnShelf(KITAP *kitapList)
{
    KITAP *b = kitapList;
    int foundAny = 0;
    printf("\n--- Şu Anda Rafta Olan Kitaplar ---\n");

    while(b != NULL){
        KITAPCOPY* c = b->copies;
        while(c != NULL){
            if(strcmp(c->status, "RAFTA") == 0){
                printf("Kitap: %s (ISBN: %s), Kopya Etiketi: %s\n", b->name, b->isbn, c->label);
                foundAny = 1;
            }
            c = c->next;
        }
        b = b->next;
    }
    if(!foundAny) printf("Şu anda rafta kitap yok.\n");
    printf("-------------------------------\n");
}

void listOverdueBooks(ODUNCKAYDI* oduncList, KITAP* kitapList, OGRENCI* ogrenciList)
{
    char currentDateStr[DATE_LEN];
    getCurrentDate(currentDateStr);
    int foundAny = 0;

    printf("\n--- Gecikmiş Kitaplar (15 günden fazla ödünç alınmış ve iade edilmemiş) ---\n");

    KITAP* bk = kitapList;
    while(bk != NULL){
        KITAPCOPY* cp = bk->copies;
        while(cp != NULL){
            if(strcmp(cp->status, "RAFTA") != 0){ // If the copy is borrowed
                int studentNum = atoi(cp->status);
                OGRENCI* o = findOgrenciByNumber(ogrenciList, studentNum);

                // Find the latest borrow record for this specific copy by this student
                ODUNCKAYDI* latestBorrowForThisCopyByStudent = NULL;
                ODUNCKAYDI* k_search = oduncList;
                while(k_search != NULL){
                    if(k_search->OgrenciNumber == studentNum &&
                       strcmp(k_search->kitapEtiketNo, cp->label) == 0 &&
                       k_search->islemTuru == 0){ // It's a borrow
                        // Check if this borrow is more recent
                        if(latestBorrowForThisCopyByStudent == NULL ||
                           calculateDaysDifference(latestBorrowForThisCopyByStudent->tarih, k_search->tarih) > 0) {
                            latestBorrowForThisCopyByStudent = k_search;
                        }
                    }
                    k_search = k_search->next;
                }

                if(latestBorrowForThisCopyByStudent != NULL){
                    // Now, ensure this borrow hasn't been returned
                    int wasReturned = 0;
                    ODUNCKAYDI* returnSearch = oduncList;
                    while(returnSearch != NULL && !wasReturned) {
                        if (returnSearch->OgrenciNumber == studentNum &&
                            strcmp(returnSearch->kitapEtiketNo, cp->label) == 0 &&
                            returnSearch->islemTuru == 1 && // It's a return
                            calculateDaysDifference(latestBorrowForThisCopyByStudent->tarih, returnSearch->tarih) >=0) {
                                wasReturned = 1;
                            }
                        if (!wasReturned) returnSearch = returnSearch->next;
                    }


                    if (!wasReturned) { // Only if the latest borrow was NOT returned
                        int daysDiff = calculateDaysDifference(latestBorrowForThisCopyByStudent->tarih, currentDateStr);
                        if(daysDiff > 15){
                            printf("Öğrenci: %s %s (No: %d) | Kitap: %s (Etiket: %s) | Ödünç Alma: %s (%d gün gecikmiş)\n",
                                o ? o->name : "N/A", o ? o->surName : "", studentNum,
                                bk->name, cp->label,
                                latestBorrowForThisCopyByStudent->tarih, daysDiff - 15);
                            foundAny = 1;
                        }
                    }
                }
            }
            cp = cp->next;
        }
        bk = bk->next;
    }
    if(!foundAny) printf("Şu anda gecikmiş kitap yok.\n");
    printf("---------------------------------------------------------\n");
}


// --- Menu and Main ---
void displayMenu()
{
    printf("\n===== Kütüphane Yönetim Sistemi =====\n");
    printf("ÖĞRENCİ İŞLEMLERİ:\n");
    printf("1.  Öğrenci Ekle\n");
    printf("2.  Öğrenci Sil\n");
    printf("3.  Öğrenci Güncelle\n");
    printf("4.  Öğrenci Bilgisi Görüntüle (Numara ile)\n");
    printf("5.  Öğrenci Bilgisi Görüntüle (Ad Soyad ile)\n");
    printf("6.  Kitap Teslim Etmemiş Öğrencileri Listele\n");
    printf("7.  Cezalı Öğrencileri Listele (Puan < 100)\n");
    printf("8.  Tüm Öğrencileri Listele\n");
    printf("KİTAP İŞLEMLERİ:\n");
    printf("9.  Kitap Ekle\n");
    printf("10. Kitap Sil\n");
    printf("11. Kitap Güncelle\n");
    printf("12. Kitap Bilgisi Görüntüle (Ad ile)\n");
    printf("13. Tüm Kitapları ve Kopyalarını Listele\n");
    printf("14. Raftaki Kitapları Listele\n");
    printf("15. Zamanında Teslim Edilmeyen Kitapları Listele\n");
    printf("16. Kitap Ödünç Al\n");
    printf("17. Kitap Teslim Et\n");
    printf("YAZAR İŞLEMLERİ:\n");
    printf("18. Yazar Ekle\n");
    printf("19. Yazar Sil\n");
    printf("20. Yazar Güncelle\n");
    printf("21. Yazar Bilgisi Görüntüle (ID ile)\n");
    printf("22. Yazar Bilgisi Görüntüle (Ad Soyad ile)\n");
    printf("23. Tüm Yazarları Listele\n");
    printf("İLİŞKİ İŞLEMLERİ:\n");
    printf("24. Kitap-Yazar Eşleştir\n");
    printf("25. Kitabın Yazarını Güncelle (Ekle/Sil)\n");
    printf("26. Kitap-Yazar İlişkilerini Listele\n");
    printf("0.  Çıkış\n");
    printf("Seçiminizi girin: ");
}

int main()
{
    YAZAR *yazarListHead = NULL;
    OGRENCI *ogrenciListHead = NULL;
    KITAP *kitapListHead = NULL;
    KITAPYAZAR_REL *kitapYazarRelations = NULL;
    int kyRelationCount = 0;
    ODUNCKAYDI *oduncListHead = NULL;
    int choice;

    // Define file names locally in main
    const char *YAZARLAR_FILENAME = "yazarlar.csv";
    const char *OGRENCILER_FILENAME = "ogrenciler.csv";
    const char *KITAPLAR_FILENAME = "kitaplar.csv";
    const char *KITAPYAZAR_FILENAME = "kitapyazar.csv";
    const char *ODUNCTAKIP_FILENAME = "odunctakip.csv";

    // Load data
    loadOduncKayitlari(&oduncListHead, ODUNCTAKIP_FILENAME);
    loadYazarlar(&yazarListHead, YAZARLAR_FILENAME);
    loadOgrenciler(&ogrenciListHead, OGRENCILER_FILENAME);
    loadKitaplar(&kitapListHead, oduncListHead, KITAPLAR_FILENAME); // Depends on oduncListHead
    loadKitapYazarRelations(&kitapYazarRelations, &kyRelationCount, KITAPYAZAR_FILENAME);

    do{
        displayMenu();
        if(scanf("%d", &choice) != 1){
            printf("Geçersiz giriş. Lütfen bir numara girin.\n");
            while(getchar()!='\n'); // Clear invalid input
            choice = -1; // Set to an invalid choice to re-prompt
        }
        getchar(); // Consume the newline after scanf

        switch(choice){
            // Ogrenci
            case 1: insertOgrenci(&ogrenciListHead, OGRENCILER_FILENAME); break;
            case 2: deleteOgrenci(&ogrenciListHead, OGRENCILER_FILENAME); break;
            case 3: updateOgrenci(ogrenciListHead, OGRENCILER_FILENAME); break;
            case 4: listStudentInfoAndBorrowsByNumber(ogrenciListHead, oduncListHead); break;
            case 5: listStudentInfoAndBorrowsByName(ogrenciListHead, oduncListHead); break;
            case 6: listStudentsWhoHaventReturnBooks(kitapListHead, ogrenciListHead); break;
            case 7: listPenalizedStudents(ogrenciListHead); break;
            case 8: listOgrenciler(ogrenciListHead); break;
            // Kitap
            case 9: addBook(&kitapListHead, KITAPLAR_FILENAME); break;
            case 10: deleteKitap(&kitapListHead, &kitapYazarRelations, &kyRelationCount, KITAPLAR_FILENAME, KITAPYAZAR_FILENAME); break;
            case 11: updateKitap(kitapListHead, KITAPLAR_FILENAME); break;
            case 12: listKitapInfoByName(kitapListHead); break;
            case 13: listAllKitaplarWithCopies(kitapListHead); break;
            case 14: listBooksOnShelf(kitapListHead); break;
            case 15: listOverdueBooks(oduncListHead, kitapListHead, ogrenciListHead); break;
            case 16: borrowKitap(ogrenciListHead, kitapListHead, &oduncListHead, ODUNCTAKIP_FILENAME, OGRENCILER_FILENAME); break;
            case 17: returnKitap(ogrenciListHead, kitapListHead, &oduncListHead, ODUNCTAKIP_FILENAME, OGRENCILER_FILENAME); break;
            // Yazar
            case 18: insertYazar(&yazarListHead, YAZARLAR_FILENAME); break;
            case 19: deleteYazar(&yazarListHead, &kitapYazarRelations, &kyRelationCount, YAZARLAR_FILENAME, KITAPYAZAR_FILENAME); break;
            case 20: updateYazar(yazarListHead, YAZARLAR_FILENAME); break;
            case 21: listYazarInfoAndBooksById(yazarListHead, kitapListHead, kitapYazarRelations, kyRelationCount); break;
            case 22: listYazarInfoAndBooksByName(yazarListHead, kitapListHead, kitapYazarRelations, kyRelationCount); break;
            case 23: listYazarlar(yazarListHead); break;
            // Iliski
            case 24: addKitapYazarRelation(&kitapYazarRelations, &kyRelationCount, kitapListHead, yazarListHead, KITAPYAZAR_FILENAME); break;
            case 25: updateBookAuthors(kitapListHead, yazarListHead, &kitapYazarRelations, &kyRelationCount, KITAPYAZAR_FILENAME); break;
            case 26: listKitapYazarRelations(kitapYazarRelations, kyRelationCount); break;

            case 0: printf("Programdan çıkılıyor.\n"); break;
            default: printf("Geçersiz seçim. Lütfen tekrar deneyin.\n");
        }
    } while (choice != 0);

    // Free allocated memory
    YAZAR *yz = yazarListHead, *nextYz;
    while(yz != NULL) { nextYz = yz->next; free(yz); yz = nextYz; }

    OGRENCI *og = ogrenciListHead, *nextOg;
    while(og != NULL) { nextOg = og->next; free(og); og = nextOg; }

    KITAP *kt = kitapListHead, *nextKt;
    while(kt != NULL) { nextKt = kt->next; freeKitapCopies(kt->copies); free(kt); kt = nextKt; }

    free(kitapYazarRelations); // This was dynamically allocated with realloc
    freeOduncList(oduncListHead);

    printf("Tüm veri dosyaları kaydedilmiş olmalı. Program sonlandırıldı.\n");
    return 0;
}
