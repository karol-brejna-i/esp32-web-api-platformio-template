import datetime

FILENAME_VERSION_NO = 'versioning_version'
FILENAME_BUILD_NO   = 'versioning_build'
FILENAME_VERSION_H  = 'include/build.h'

# read version number from file
version_no = '0.0.0'
try:
    with open(FILENAME_VERSION_NO) as f:
        line = f.readline()
        # validate the version?
        version_no = line
except:
    print('Starting version number is 0.0.0')
    version_no = '0.0.0'

# increment build number
build_no = 0
try:
    with open(FILENAME_BUILD_NO) as f:
        build_no = int(f.readline()) + 1
except:
    print('Starting build number from 1')
    build_no = 1

# store build number
with open(FILENAME_BUILD_NO, 'w+') as f:
    f.write(str(build_no))
    print(f'Build number: {build_no}')

# construct version
tm = datetime.datetime.today()
long_timestamp = datetime.datetime.now()
short_timestamp = str(tm.year)[-2:] + ('0' + str(tm.month))[-2:] + ('0' + str(tm.day))[-2:]
build_version = f'{version_no}.{short_timestamp}.{build_no}'
version_json = f'{{\\"version\\": \\"{version_no}\\", \\"buildVersion\\": \\"{build_version}\\", \\"timestamp\\": \\"{long_timestamp}\\"}}'

hf = """
#ifndef BUILD_NUMBER
  #define BUILD_NUMBER "{}"
#endif

#ifndef VERSION
  #define VERSION "{}"
#endif

#ifndef BUILD_VERSION
  #define BUILD_VERSION "{}"
#endif

#ifndef BUILD_TIMESTAMP
  #define BUILD_TIMESTAMP "{}"
#endif

#ifndef VERSION_JSON
  #define VERSION_JSON "{}"
#endif
""".format(build_no, version_no, build_version, long_timestamp, version_json)

print(hf)

# store version in header file
with open(FILENAME_VERSION_H, 'w+') as f:
    f.write(hf)
