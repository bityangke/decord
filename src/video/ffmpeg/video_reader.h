/*!
 *  Copyright (c) 2019 by Contributors if not otherwise specified
 * \file video_reader.h
 * \brief FFmpeg video reader, implements VideoReaderInterface
 */

#ifndef DECORD_VIDEO_FFMPEG_VIDEO_READER_H_
#define DECORD_VIDEO_FFMPEG_VIDEO_READER_H_

#include "threaded_decoder.h"
#include <decord/video_interface.h>

#include <string>
#include <vector>

#include <decord/base.h>
#include <dmlc/concurrency.h>


namespace decord {
namespace ffmpeg {

class FFMPEGVideoReader : public VideoReaderInterface {
    using FFMPEGThreadedDecoderPtr = std::unique_ptr<FFMPEGThreadedDecoder>;
    using NDArray = runtime::NDArray;
    public:
        FFMPEGVideoReader(std::string fn, int width=-1, int height=-1);
        /*! \brief Destructor, note that FFMPEG resources has to be managed manually to avoid resource leak */
        ~FFMPEGVideoReader();
        void SetVideoStream(int stream_nb = -1);
        unsigned int QueryStreams() const;
        int64_t GetFrameCount() const;
        NDArray NextFrame();
        NDArray GetBatch(std::vector<int64_t> indices);
        void SkipFrames(int64_t num = 1);
        void PushNext();
        bool Seek(int64_t pos);
        bool SeekAccurate(int64_t pos);
        runtime::NDArray GetKeyIndices();
    protected:
        friend class FFMPEGVideoLoader;
        std::vector<int64_t> GetKeyIndicesVector() const;
    private:
        void IndexKeyframes();
        int64_t LocateKeyframe(int64_t pos);
        AVFramePtr NextFrameImpl();
        std::vector<int64_t> key_indices_;
        /*! \brief Get or Create SwsContext by dtype */
        // struct SwsContext* GetSwsContext(FrameTransform out_fmt);
        /*! \brief Video Streams Codecs in original videos */
        std::vector<AVCodec*> codecs_;
        /*! \brief Currently active video stream index */
        int actv_stm_idx_;
        /*! \brief AV format context holder */
        AVFormatContextPtr fmt_ctx_;
        /*! \brief AVPacket buffer */
        // AVPacket *pkt_;
        /*! \brief AVFrame buffer */
        // AVFrame *frame_;
        /*! \brief AV dodec context for decoding related info */
        // AVCodecContext *dec_ctx_;
        /*! \brief Container for various FFMPEG swsContext */
        // std::unordered_map<FrameTransform, struct SwsContext*> sws_ctx_map_;
        FFMPEGThreadedDecoderPtr decoder_;
        int64_t curr_frame_;  // current frame location
        int width_;   // output video width
        int height_;  // output video height
        bool eof_;  // end of file indicator
};  // class FFMPEGVideoReader


}  // namespace ffmpeg
}  // namespace decord

#endif  // DECORD_VIDEO_FFMPEG_VIDEO_READER_H_