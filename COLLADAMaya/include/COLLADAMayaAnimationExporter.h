/*
    Copyright (c) 2008 NetAllied Systems GmbH
	
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#ifndef __COLLADA_MAYA_ANIMATION_EXPORTER_H__
#define __COLLADA_MAYA_ANIMATION_EXPORTER_H__

#include "ColladaMayaPrerequisites.h"
#include "COLLADAMayaAnimationHelper.h"
#include "COLLADAMayaAnimationTools.h"
#include "COLLADAMayaDocumentExporter.h"
#include "COLLADAMayaAnimationSampleCache.h"
#include "COLLADAMayaAnimationElement.h"
#include "COLLADAMayaSceneElement.h"

#include <maya/MFnAnimCurve.h>

#include "COLLADALibraryAnimations.h"
#include "COLLADASource.h"

namespace COLLADAMaya
{

    // ----------------------------------------------------------
    /**
     * Exports all animation data of the current scene.
     */

    class AnimationExporter : public COLLADA::LibraryAnimations
    {

    private:

        /** Parameter, used for the transforms, if we have a matrix source with dimension 32. */
        static const String PARAM_TYPE_TRANSFORM;

        /** Parameter, used for the transforms, if we have a matrix source with dimension 16. */
        static const String PARAM_TYPE_X_Y;

        /** The time parameter for the animation. */
        static const String PARAM_TYPE_TIME;

        /** The interpolation parameter for the animation. */
        static const String PARAM_TYPE_INTERPOLATION;

        /** The tcb parameter for animations. */
        static const String PARAM_TYPE_TCBS;

        /** The eases parameter for the animation. Used with tcbs. */
        static const String PARAM_TYPE_EASES;

        /** Pointer to the main document exporter. */
        DocumentExporter* mDocumentExporter;

        /** True, if we have an import. */
        bool isImport;

        /** The list with the animated elements. */
        AnimatedElementList mAnimationElements;

        /** The list with the animation clips. */
        AnimationClipList mAnimationClips;

    public:

        /**
         * Constructor.
         * @param streamWriter Pointer to the collada stream writer.
         * @param documentExporter Pointer to the main document exporter.
         */
        AnimationExporter ( COLLADA::StreamWriter* streamWriter, DocumentExporter* documentExporter );

        /**
         * Destructor.
         */
        virtual ~AnimationExporter();

        /**
         * Exports all animations from the member animation list.
         * @return const AnimationClipList*
         *   Returns a const pointer to the animation clip list,
         *   managed and released in the class @AnimationExporter
         */
        const AnimationClipList* exportAnimations();

        /**
         * Searches the plug with the given attribute of the given node
         * and adds the plug with the given sample type to the list of animations,
         * if it is a valid animation. Takes the attribute name as the sub id.
         * @param node The node, from which we want to export the attribute
         * @param attrname The name of the attribute, we want to export.
         * @param parameters The list of the parameters of the attribute.
         * @param sampleType The sample type, which means the dimension of the attribute to export.
         * @param conversion The conversion functor, if we have to convert the values of the attribute.
         */
        void addPlugAnimation ( MObject &node,
                                const String attrname,
                                const String* parameters,
                                const uint sampleType,
                                ConversionFunctor* conversion = NULL );

        /**
         * Searches the plug with the given attribute of the given node
         * and adds the plug with the given sample type to the list of animations,
         * if it is a valid animation.
         * @param node The node, from which we want to export the attribute
         * @param subId The id of the attribute to export.
         * @param attrname The name of the attribute, we want to export.
         * @param parameters The list of the parameters of the attribute.
         * @param sampleType The sample type, which means the dimension of the attribute to export.
         * @param conversion The conversion functor, if we have to convert the values of the attribute.
         */
        void addPlugAnimation ( MObject& node,
                                const String subId,
                                const String attrname,
                                const String* parameters,
                                const uint sampleType,
                                ConversionFunctor* conversion = NULL );

        /**
         * Adds the plug with the given sample type to the list of animations,
         * if it is a valid animation.
         * @param plug The plug which is compound with the searched attribute.
         * @param subId The id of the attribute to export.
         * @param parameters The list of the parameters of the attribute.
         * @param sampleType The sample type, which means the dimension of the attribute to export.
         * @param conversion The conversion functor, if we have to convert the values of the attribute.
         */
        void addPlugAnimation ( MPlug& plug,
                                const String subId,
                                const String* parameters,
                                const SampleType sampleType,
                                ConversionFunctor* conversion = NULL );

        /**
         * Adds the plug with the given sample type to the list of animations,
         * if it is a valid animation.
         * @param plug The plug which is compound with the searched attribute.
         * @param subId The id of the attribute to export.
         * @param parameters The list of the parameters of the attribute.
         * @param sampleType The sample type, which means the dimension of the attribute to export.
         * @param conversion The conversion functor, if we have to convert the values of the attribute.
         */
        void addPlugAnimation ( MPlug& plug,
                                const String subId,
                                const String* parameters,
                                const uint sampleType,
                                ConversionFunctor* conversion = NULL );

    private:

        /** Go through the scene graph and look for IK-Handles to sample. */
        void lookForSamples();

        /** Checks the current scene element and all it's children for sampling as IK-Handle. */
        void lookForSamples ( SceneElement* sceneElement );

        /**
        * Exports all animation curves of the animated elements and the child
         * elements in the given list recursive.
         * @param animatedElements List with the animated elements to export.
         */
        void exportAnimatedElements ( const AnimatedElementList& animatedElements );

        /**
         * Exports the data of a multi curve. This is possible if we use bake matrix on export,
         * or if we can merge some curves, because they are the same.
         * @param animationElement The animated element.
         * @param defaultValues List with the default values.
         */
        void exportAnimatedMultiCurve ( AnimationElement* animatedElement,
                                        std::vector<float>& defaultValues );

        /** Create the curves for the samples */
        void postSampling();

        /**
         * Export a scene element animation.
         * @param animatedElement The animated element to export.
         * @return bool True, if the export was successfull.
         */
        bool exportAnimation ( AnimationElement* animatedElement );

        /**
         * Export the data of every curve in the animated element.
         * @param animatedElement The animated element to export.
         */
        void exportAnimatedCurves ( const AnimationElement* animatedElement );

        /**
        * Create a merged curve from the input curves. The default values have to be filled.
         * @param animatedElement The animated element to merge.
         * @param defaultValues The list with the default values.
         * @return AnimationMultiCurve* Pointer to the created multi curve.
         *   You have to manage and destroy the created element.
         */
        AnimationMultiCurve* createAnimationMultiCurve ( AnimationElement* animatedElement,
                std::vector<float> &defaultValues );

        /**
         * No curve, or an empty curve, set the default value on all the keys.
         * @param keys Pointer to the list of keys.
         * @param curvePosition Position of the current curve in the list of curves.
         * @param defaultValues List with default values to create the keys.
         * @param keyCount Number of keys of the curve.
         * @param mergedInputs The merged input values.
         */
        void createDefaultKeys ( AnimationMKeyList* keys,
                                 uint curvePosition,
                                 std::vector<float> &defaultValues,
                                 const uint keyCount,
                                 const std::vector<float> mergedInputs );

        /**
         * Merge in this curve's values, sampling when the multi-curve's
         * key is not present in the curve.
         * Calculate and retrieve the tangents in a polar-like form.
         * @param curve The curve to merge in.
         * @param keyCount The number of keys in the curve.
         * @param keys The list of keys we want to merge.
         * @param curvePosition The position of the current curve.
         */
        void mergeCurves ( AnimationMKeyList* keys,
                           const AnimationCurve* curve,
                           uint keyCount,
                           uint curvePosition );

        /**
         * Creates a TCB multi key.
         * @param key The base key.
         * @param curveKeys List of curve keys.
         * @param curvePosition Position of the current curve.
         * @param keyPosition Position of the key.
         */
        void createTCBMKey ( AnimationMKey* key,
                             const AnimationKeyList& curveKeys,
                             const uint curvePosition,
                             const uint keyPosition );

        /**
         * Creates a bezier multi key.
         * @param key The base key.
         * @param curveKeys List of curve keys.
         * @param curvePosition Position of the current curve.
         * @param previousSpan The span in front of the current key.
         * @param nextSpan The span after the current key.
         * @param keyPosition Position of the key.
         */
        void createBezierMKey ( AnimationMKey* key,
                                const AnimationKeyList& curveKeys,
                                const uint curvePosition,
                                const float previousSpan,
                                const float nextSpan,
                                const uint keyPosition );

        /**
         * Sample the curve key.
         * @param key Key to sample.
         * @param curve Curve to sample with.
         * @param curvePosition Position of the curve in list.
         * @param input Input of the curve.
         * @param previousSpan The span in front of the key.
         * @param nextSpan The span behind the key.
         */
        void sampleMKey ( AnimationMKey* key,
                          const AnimationCurve* curve,
                          const uint curvePosition,
                          const float input,
                          const float previousSpan,
                          const float nextSpan );

        /**
         * Check if the curves in the animated element can be merged.
         * All curves must have the same inputs, interpolations and infinity types.
         * If the curves are merge able, the std::vector with the default values
         * will be filled and returned.
         * @param animationElement The animated element.
         * @param defaultValues List with the default values.
         * @return bool True, if the curves of the element are merge able.
         */
        bool curvesAreMergeable ( const AnimationElement* animationElement,
                                  std::vector<float>* defaultValues );

        /**
         * Check if the curve can be merged with the master curve.
         * All curves must have the same inputs, interpolations and infinity types.
         * @param masterCurve The master curve.
         * @param curve The curve to merge.
         * @return bool True, if the curves are mergeable.
         */
        bool curveIsMergeable ( const AnimationCurve* masterCurve,
                                const AnimationCurve* curve );

        /**
         * Create the default values vector from the master curve keys.
         * @param masterCurve the master curve with the default keys.
         * @param defaultValues the list with the default values to fill.
         * @return std::vector<float>* the filled list.
         */
        void createDefaultValues ( const AnimationCurve* masterCurve,
                                   std::vector<float>* defaultValues );

        /**
         * Creates a animation curve of an animated element.
         * @param animatedElement The animated element.
         * @param plug The plug with the curves.
         * @param sampleType The sample type for the dimension.
         * @param conversion The conversion functor to convert the values.
         * @param curves The list to push the created curves .
         * @param curveIndex The index of the current curve, if it is a animated compound attribute.
         * @return bool True, if a curve was created.
         */
        bool createAnimationCurve ( AnimationElement* animatedElement,
                                    const MPlug& plug,
                                    const SampleType sampleType,
                                    ConversionFunctor* conversion,
                                    AnimationCurveList& curves,
                                    const uint curveIndex=0 );

        /**
         * Verify that there is, in fact, an animation in this curve.
         * @param curves List of curves to compare.
         * @return bool True, if there is no animation.
         */
        bool curvesAreEqual ( const AnimationCurveList &curves );

        /**
         * Creates a animation curve of an collada channel.
         * @param animatedElement The animated element.
         * @param curveObject The maya curve object.
         * @param baseId The base id of the curve to create.
         * @param curveIndex The index of the current curve, if it is a animated compound attribute.
         * @return AnimationCurve* Pointer to the created curve. You have to destroy.
         */
        AnimationCurve* createAnimationCurveFromNode ( AnimationElement* animatedElement,
                const MObject& curveObject,
                const String& baseId,
                const uint curveIndex );

        /**
          * Search in the list of animated elements for the given plug.
         * Checks the known plugs and their child plugs.
         * Set the index of the plug, if it is compound (child index).
         * @param plug The Plug for search.
         * @param index The index of the plug to fill.
         * @return AnimationElement* Pointer to the animated element with the plug.
         */
        AnimationElement* findAnimated ( const MPlug& plug, int& index );

        /**
         * Search for the given plug in the animated element and in the child elements.
         * Set the index of the plug, if it is compound (child index).
         * @param plug The Plug for search.
         * @param index The index of the plug to fill.
         * @param AnimationElement* Pointer to the animated element to search.
         * @return true, if the plug was found in the list.
         */
        bool findAnimated ( const MPlug& plug, int& index, AnimationElement* animation );

        /**
         * Create the keys of the current curve.
         * @param animCurveFn The maya curve object.
         * @param curve The animated curve.
         * @param animatedElement The animated element.
         */
        void createAnimationCurveKeys ( MFnAnimCurve &animCurveFn,
                                        AnimationCurve* curve,
                                        AnimationElement* animatedElement );

        /**
         * Creates a bezier key element.
         * @param key The main key element.
         * @param animCurveFn The maya curve object.
         * @param keyPosition The position of the current key.
         * @param keyCount The number of keys in the curve.
         */
        void createBezierKey ( AnimationKey* key,
                               MFnAnimCurve &animCurveFn,
                               uint keyPosition,
                               uint keyCount );

        /**
         * Create an animation curve from the plug's clip.
         * @param animatedElement The animated element.
         * @param plug The plug with the clips.
         * @param conversion The conversion functor to convert the values.
         * @param curves List of curves to insert created curves.
         * @param curveIndex The index of the current curve, if it is a compound object.
         * @return bool True, if a curve was created.
         */
        bool createAnimationCurveFromClip ( AnimationElement* animatedElement,
                                            const MPlug& plug,
                                            ConversionFunctor* conversion,
                                            AnimationCurveList& curves,
                                            const uint curveIndex );

        /**
         * Retrieve/Create the clips associated with a character node.
         * @param characterNode The maya node object.
         * @param characterClips List of clips.
         */
        void getCharacterClips ( const MObject& characterNode,
                                 AnimationClipList& characterClips );

        /**
         * Writes the sampler of the animation curve in the collada document.
         * @param animationCurve The animated curve to export.
         */
        template<class T>
        void exportAnimationSampler ( const BaseAnimationCurve<T> &animationCurve );

        /**
         * Writes the channel of the animation curve in the collada document.
         * @param animationCurve The animated curve to export.
         */
        template<class T>
        void exportAnimationChannel ( const BaseAnimationCurve<T> &animationCurve );

        /**
         * Writes the curve in the collada document.
         * @param animationCurve The curve to export.
         */
        void exportAnimationSource ( AnimationCurve &animationCurve );

        /**
        * Writes the curve in the collada document.
        * @param animationCurve The curve to export.
        */
        void exportAnimationSource ( AnimationMultiCurve &animationCurve );

        /**
         * Writes the input, output, tangents and tcbs in the collada document.
         * @param animationCurve The animated curve.
         * @param input The input values.
         * @param output The output values.
         * @param interpolations The interpolation values.
         * @param inTangents The input tangent values.
         * @param outTangents The output tangent values.
         * @param tcbs The tcb values.
         * @param eases The ease values.
         */
        template<class T>
        void writeAnimationSource ( const BaseAnimationCurve<T>& animationCurve,
                                    const std::vector<float>& input,
                                    const std::vector<float>& output,
                                    const std::vector<String>& interpolations,
                                    const std::vector<float>& inTangents,
                                    const std::vector<float>& outTangents,
                                    const std::vector<float>& tcbs,
                                    const std::vector<float>& eases );

        /**
         * Fills the tcb and ease vectors for the collada output source.
         * @param key The animation multi key.
         * @param dimension The dimension of the animation.
         * @param tcbs The list for the tcbs.
         * @param eases The list for the eases.
         */
        void createMultiCurveTCBs ( AnimationMKey* key,
                                    uint dimension,
                                    std::vector<float> &tcbs,
                                    std::vector<float> &eases );

        /**
         * Fills the tangent vectors for the collada output source.
         * @param key The animation multi key.
         * @param dimension The dimension of the animation.
         * @param inTangents The list for the input tangent values.
         * @param outTangents The list for the output tangent values.
         * @param outputList The list with the output values.
         */
        void createMultiCurveTangents ( AnimationMKey* key,
                                        uint dimension,
                                        std::vector<float> &inTangents,
                                        std::vector<float> &outTangents,
                                        float* outputList );

        /**
         * Writes a input source in the collada document.
         * @param sourceId The source id.
         * @param values The values to export.
         * @param preInfinityType The interpolation type in front of the curve.
         * @param postInfinityType The interpolation type after the curve.
         */
        void exportInputSource ( const String sourceId,
                                 const std::vector<float> &values,
                                 String preInfinityType,
                                 String postInfinityType );

        /**
         * Writes a output source in the collada document.
         * @param sourceId The source id.
         * @param parameters The parameters for the output.
         * @param dimension The dimension of the curve.
         * @param values Values to export.
         */
        void exportOutputSource ( const String sourceId,
                                  const String* parameters,
                                  const uint dimension,
                                  const std::vector<float> &values );

        /**
         * Writes a interpolation source in the collada document.
         * @param sourceId The source id.
         * @param interpolations List of export values.
         */
        void exportInterpolationSource ( const String sourceId,
                                         const std::vector<String> interpolations );

        /**
         * Writes a in tangent source in the collada document.
         * @param sourceId The source id.
         * @param dimension The dimension of the curve.
         * @param inTangent List of export values.
         */
        void exportInTangentSource ( const String sourceId,
                                     const uint dimension,
                                     const std::vector<float> &inTangent );

        /**
         * Writes a out tangent source in the collada document.
         * @param sourceId The source id.
         * @param dimension The dimension of the curve.
         * @param outTangent List of export values.
         */
        void exportOutTangentSource ( const String sourceId,
                                      const uint dimension,
                                      const std::vector<float> &outTangent );


        /**
         * Writes a tangent source in the collada document.
         * @param sourceId The source id.
         * @param sourceIdSuffix The source id suffix.
         * @param dimension The dimension of the curve.
         * @param values List of export values.
         */
        void exportTangentSource ( const String sourceId,
                                   const String sourceIdSuffix,
                                   const uint dimension,
                                   const std::vector<float> &values );

        /**
         * Writes a TCB source in the collada document.
         * @param sourceId The source id.
         * @param values List of export values.
         */
        void exportTCBSource ( const String sourceId,
                               const std::vector<float> &values );

        /**
         * Writes the eases source in the collada document.
         * @param sourceId The source id.
         * @param values List of export values.
         */
        void exportEasesSource ( const String sourceId,
                                 const std::vector<float> &values );

        /**
         * Returns the base id of the plug.
         * @param plug The maya plug.
         * @return COLLADAMaya::String Returns the base id of the plug.
         */
        String getBaseId ( const MPlug &plug );

        /**
         * Returns the node id of the plug.
         * @param plug The maya plug.
         * @return COLLADAMaya::String Returns the node id of the plug.
         */
        String getNodeId ( const MPlug &plug );

        /**
         * Build the target name and returns it.
         * @param animationCurve The animated curve.
         * @return COLLADAMaya::String The target name of the curve.
         */
        template<class T>
        String getTarget ( const BaseAnimationCurve<T> &animationCurve );

    };

}

#endif // __COLLADA_MAYA_ANIMATION_EXPORTER_H__
