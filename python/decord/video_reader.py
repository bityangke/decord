"""Video Reader."""
from __future__ import absolute_import

import ctypes
import numpy as np

from ._ffi.base import c_array, c_str
from ._ffi.function import _init_api
from .base import DECORDError
from . import ndarray as _nd

VideoReaderHandle = ctypes.c_void_p


class VideoReader(object):
    def __init__(self, uri, width=-1, height=-1):
        self._handle = None
        self._handle = _CAPI_VideoReaderGetVideoReader(
            uri, width, height)
        self._num_frame = _CAPI_VideoReaderGetFrameCount(self._handle)
        assert self._num_frame > 0
    
    @property
    def num_frame(self):
        return self._num_frame

    def next(self):
        assert self._handle is not None
        arr = _CAPI_VideoReaderNextFrame(self._handle)
        if not arr.shape:
            raise StopIteration()
        return arr
    
    def get_key_indices(self):
        assert self._handle is not None
        indices = _CAPI_VideoReaderGetKeyIndices(self._handle)
        if not indices.shape:
            raise RuntimeError("No key frame indices found.")
        return indices
    
    def seek(self, pos):
        assert self._handle is not None
        assert pos >= 0 and pos < self._num_frame
        success = _CAPI_VideoReaderSeek(self._handle, pos)
        if not success:
            raise RuntimeError("Failed to seek to frame {}".format(pos))
    
    def seek_accurate(self, pos):
        assert self._handle is not None
        assert pos >= 0 and pos < self._num_frame
        success = _CAPI_VideoReaderSeekAccurate(self._handle, pos)
        if not success:
            raise RuntimeError("Failed to seek_accurate to frame {}".format(pos))

    def skip_frames(self, num=1):
        assert self._handle is not None
        assert num > 0
        _CAPI_VideoReaderSkipFrames(self._handle, num)

_init_api("decord.video_reader")