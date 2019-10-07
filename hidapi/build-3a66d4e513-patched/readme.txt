These binaries were built from hidapi git commit 3a66d4e513, with the following
patch applied. The patch has been submitted upstream.



From ba5f2772310c82970f539af186bbb11f99c08377 Mon Sep 17 00:00:00 2001
From: "Mark A. Tsuchida" <marktsuchida@gmail.com>
Date: Sat, 21 Dec 2013 13:01:28 -0800
Subject: windows: limit hid_read() return value to buffer length

hid_read() and hid_read_timeout() now return the number of bytes copied,
instead of the number of bytes returned by GetOverlappedResult() (which is the
maximum report size for the device). This limits the return value to the
requested length (buffer size), matching the behavior on other platforms.
---
 windows/hid.c | 6 +++---
 1 file changed, 3 insertions(+), 3 deletions(-)

diff --git a/windows/hid.c b/windows/hid.c
index 3dee05b..89d07b1 100755
--- a/windows/hid.c
+++ b/windows/hid.c
@@ -652,6 +652,7 @@ end_of_function:
 int HID_API_EXPORT HID_API_CALL hid_read_timeout(hid_device *dev, unsigned char *data, size_t length, int milliseconds)
 {
 	DWORD bytes_read = 0;
+	size_t copy_len = 0;
 	BOOL res;
 
 	/* Copy the handle for convenience. */
@@ -699,14 +700,13 @@ int HID_API_EXPORT HID_API_CALL hid_read_timeout(hid_device *dev, unsigned char
 			   number (0x0) on the beginning of the report anyway. To make this
 			   work like the other platforms, and to make it work more like the
 			   HID spec, we'll skip over this byte. */
-			size_t copy_len;
 			bytes_read--;
 			copy_len = length > bytes_read ? bytes_read : length;
 			memcpy(data, dev->read_buf+1, copy_len);
 		}
 		else {
 			/* Copy the whole buffer, report number and all. */
-			size_t copy_len = length > bytes_read ? bytes_read : length;
+			copy_len = length > bytes_read ? bytes_read : length;
 			memcpy(data, dev->read_buf, copy_len);
 		}
 	}
@@ -717,7 +717,7 @@ end_of_function:
 		return -1;
 	}
 	
-	return bytes_read;
+	return copy_len;
 }
 
 int HID_API_EXPORT HID_API_CALL hid_read(hid_device *dev, unsigned char *data, size_t length)
-- 
1.8.4.msysgit.0

