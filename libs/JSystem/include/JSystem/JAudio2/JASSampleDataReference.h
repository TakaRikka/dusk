#ifndef DUSKLIGHT_JASSAMPLEDATAREFERENCE_H
#define DUSKLIGHT_JASSAMPLEDATAREFERENCE_H

#if TARGET_PC
/**
 * An object to manage the lifetime of the sample data pointed to by JASChannel.
 *
 * JASChannel can (optionally) accept an unique_ptr to an object to this type,
 * in which case it will be destroyed when the JASChannel is done.
 *
 * @see JASChannel::mSampleReference
 */
struct JASSampleDataReference {
    virtual ~JASSampleDataReference() = default;
};
#endif

#endif  // DUSKLIGHT_JASSAMPLEDATAREFERENCE_H
