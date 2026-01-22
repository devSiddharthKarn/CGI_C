# CGI_C Release Notes

## Version 2.0.0 - January 22, 2026

### 🎉 Major Features

#### Font Rendering System
- **Added CGIPen API** for advanced text rendering with TrueType font support
  - `CGIGeneratePen()` - Create a font pen from TTF files
  - `CGIWriteTextWithPen()` - Render text with full control over scaling, spacing, and positioning
  - `CGIDestroyPen()` - Clean up font resources
  - Supports horizontal and vertical scaling
  - Configurable character spacing (x_spacing, y_spacing)
  - Line wrapping support
  - Character offset for indentation
  - Anti-aliased rendering with alpha blending

#### New API Functions
- **`CGIGetPixelColor()`** - Retrieve the color value of any pixel in the window buffer
- **`CGIRefreshBufferRegion()`** - Selective buffer refresh for optimized rendering

### 🔄 Changes

#### Deprecated Functions
- **`CGIWriteText()`** - Marked as deprecated in favor of `CGIWriteTextWithPen()`
  - Still available for backward compatibility
  - Existing code will continue to work without modification
  - Users are encouraged to migrate to the new Pen API for better control and quality

#### Build System Updates
- Updated build configuration to latest linkable binary format
- Improved compilation optimization flags
- Enhanced cross-platform compatibility (Windows/Linux)

### 🐛 Bug Fixes
- Fixed glyph rendering loop bounds checking
- Corrected font scale calculation in text rendering
- Fixed memory management for font buffers
- Resolved spacing issues with space characters
- Fixed line wrapping to maintain offset indentation

### 📚 API Enhancements
- Improved window refresh modes
- Enhanced color blending for text rendering
- Better memory management in font system
- Added proper cleanup routines for all resources

### ⚠️ Breaking Changes
None - This release maintains full backward compatibility

### 📋 Migration Guide
To take advantage of the new font system:

```c
// Old way (still works)
CGIWriteText(window, "Hello", x, y, color);

// New way (recommended)
CGIPen* pen = CGIGeneratePen("arial.ttf", 48.0f);
CGIWriteTextWithPen(window, pen, "Hello", x, y, 2, 2, 1.0, 1.0, 0, CGI_true, color);
CGIDestroyPen(pen);
```

### 🔗 Dependencies
- STB TrueType library (included)
- OpenGL/GLAD (included)

---

## Previous Versions
See commit history for earlier release notes.
