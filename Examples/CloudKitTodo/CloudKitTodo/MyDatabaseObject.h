#import <Foundation/Foundation.h>

/**
 * This is an example base class that demonstrates certain concepts you'll likely find useful when using YapDatabase.
 * 
 * **** You do NOT have to use this base class. ****
 *
 * If you thought, for a moment, that YapDatabase enforces a base class like NSManagedObject (gag)
 * please banish that thought from your mind now.
 * 
 * Truth be told, YapDatabase doesn't care what your objects look like.
 * Just so long as you're able to properly serialize & deserialize them.
 * Which means that you're more than welcome to use plain old NSObject subclasses.
 * Or maybe even an open source project like one of these:
 * 
 * - https://github.com/Mantle/Mantle
 * - https://github.com/nicklockwood/FastCoding
 * 
 * It's completely up to you!
 * 
 * That being said, I think you'll find some of the concepts demonstrated here to be useful.
 * So feel free to copy & modify anything in this class. Meld it to fit your needs.
 * Maybe you'll even merge some of the concepts in this class with other popular open source projects
 * to create your own super base class. :)
 * 
 * ***** Concept #1 *****
 * 
 * With a highly concurrent database (like YapDatabase), thread-safety becomes important.
 * But you don't want thread-safety to become burdensome. What you really want is something so simple and
 * straight-forward that you don't have to think about it (yet it's automatically thread-safe).
 * 
 * I touch on several thread-safety issues in the "Thread Safety" wiki article:
 * https://github.com/yapstudios/YapDatabase/wiki/Thread-Safety
 * 
 * But you can make life really easy on yourself if you use immutable objects.
 * Of course, you must be thinking, "easier said than done" right?
 * Or perhaps "that sounds like a pain in the ass"?
 *
 * Surprisingly, it only takes a few lines of code!
 * I explain the concept in depth here (along with a bonus performance improvement you can get from it):
 * https://github.com/yapstudios/YapDatabase/wiki/Object-Policy
 * 
 * But here's a 10,000 foot summary:
 * - Your model objects extend this one (or you rip off this class - feel free)
 * - You simply call [object makeImmutable]
 * - Voilà, you've got yourself an immutable thread-safe object,
 *   ready to go in the database and be passed around from thread-to-thread like a ... well behaved object.
 *
 * ***** Concept #2 *****
 * 
 * Tracking which properties of an object have been changed.
 *
 * The most obvious use of this is for syncing changes to a cloud based service such as CloudKit.
 * https://github.com/yapstudios/YapDatabase/wiki/YapDatabaseCloudKit
**/


/**
 * You can use this macro to fetch the CloudKit key for a given property.
 * For example, say your class is configured with the following syncablePropertyMappings:
 * @{ @"uuid"  : @"uuid"
 *    @"color" : @"ck_color"
 * }
 *
 * Then:
 * - CKKey(uuid)  => [self.syncablePropertyMappings objectForKey:@"ivar"] => @"uuid"
 * - CKKey(color) => [self.syncablePropertyMappings objectForKey:@"color"] => @"ck_color"
 * 
 * In other words, it returns the name of the corresponding ivar within the CKRecord.
**/
#define CKKey(ivar) [self.syncablePropertyMappings objectForKey:@"" # ivar]
// translation  ==> [self.syncablePropertyMappings objectForKey:@"ivar"]


@interface MyDatabaseObject : NSObject <NSCopying>

// Immutability

@property (nonatomic, readonly) BOOL isImmutable;

- (void)makeImmutable;

- (NSException *)immutableExceptionForKey:(NSString *)key;

// Tracking what changed

@property (nonatomic, readonly) NSSet *changedProperties;
@property (nonatomic, readonly) BOOL hasChangedProperties;

- (void)clearChangedProperties;

// Tracking what changed (the sync-able subset)

@property (nonatomic, readonly) NSSet *allSyncableProperties;

@property (nonatomic, readonly) NSSet *changedSyncableProperties;
@property (nonatomic, readonly) BOOL hasChangedSyncableProperties;

// Class configuration

+ (NSMutableSet *)monitoredProperties;
@property (nonatomic, readonly) NSSet *monitoredProperties;

+ (NSMutableDictionary *)syncablePropertyMappings;
@property (nonatomic, readonly) NSDictionary *syncablePropertyMappings;

@end