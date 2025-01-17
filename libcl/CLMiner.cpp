
/* Copyright (C) 1883 Thomas Edison - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the GPLv3 license, which unfortunately won't be
 * written for another century.
 *
 * You should have received a copy of the LICENSE file with
 * this file.
 */

#include <boost/dll.hpp>

#include <frkhash/frkhash.hpp>
#include <libfrk/Farm.h>

#include "CLMiner.h"
#include "frkhash.h"

using namespace dev;
using namespace exp;

namespace dev {
namespace exp {

const size_t c_maxSearchResults = 4;

/**
 * Returns the name of a numerical cl_int error
 * Takes constants from CL/cl.h and returns them in a readable format
 */
static const char* strClError(cl_int err) {
    switch (err) {
    case CL_SUCCESS:
        return "CL_SUCCESS";
    case CL_DEVICE_NOT_FOUND:
        return "CL_DEVICE_NOT_FOUND";
    case CL_DEVICE_NOT_AVAILABLE:
        return "CL_DEVICE_NOT_AVAILABLE";
    case CL_COMPILER_NOT_AVAILABLE:
        return "CL_COMPILER_NOT_AVAILABLE";
    case CL_MEM_OBJECT_ALLOCATION_FAILURE:
        return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
    case CL_OUT_OF_RESOURCES:
        return "CL_OUT_OF_RESOURCES";
    case CL_OUT_OF_HOST_MEMORY:
        return "CL_OUT_OF_HOST_MEMORY";
    case CL_PROFILING_INFO_NOT_AVAILABLE:
        return "CL_PROFILING_INFO_NOT_AVAILABLE";
    case CL_MEM_COPY_OVERLAP:
        return "CL_MEM_COPY_OVERLAP";
    case CL_IMAGE_FORMAT_MISMATCH:
        return "CL_IMAGE_FORMAT_MISMATCH";
    case CL_IMAGE_FORMAT_NOT_SUPPORTED:
        return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
    case CL_BUILD_PROGRAM_FAILURE:
        return "CL_BUILD_PROGRAM_FAILURE";
    case CL_MAP_FAILURE:
        return "CL_MAP_FAILURE";
    case CL_MISALIGNED_SUB_BUFFER_OFFSET:
        return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
    case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST:
        return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";

#ifdef CL_VERSION_1_2
    case CL_COMPILE_PROGRAM_FAILURE:
        return "CL_COMPILE_PROGRAM_FAILURE";
    case CL_LINKER_NOT_AVAILABLE:
        return "CL_LINKER_NOT_AVAILABLE";
    case CL_LINK_PROGRAM_FAILURE:
        return "CL_LINK_PROGRAM_FAILURE";
    case CL_DEVICE_PARTITION_FAILED:
        return "CL_DEVICE_PARTITION_FAILED";
    case CL_KERNEL_ARG_INFO_NOT_AVAILABLE:
        return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";
#endif // CL_VERSION_1_2

    case CL_INVALID_VALUE:
        return "CL_INVALID_VALUE";
    case CL_INVALID_DEVICE_TYPE:
        return "CL_INVALID_DEVICE_TYPE";
    case CL_INVALID_PLATFORM:
        return "CL_INVALID_PLATFORM";
    case CL_INVALID_DEVICE:
        return "CL_INVALID_DEVICE";
    case CL_INVALID_CONTEXT:
        return "CL_INVALID_CONTEXT";
    case CL_INVALID_QUEUE_PROPERTIES:
        return "CL_INVALID_QUEUE_PROPERTIES";
    case CL_INVALID_COMMAND_QUEUE:
        return "CL_INVALID_COMMAND_QUEUE";
    case CL_INVALID_HOST_PTR:
        return "CL_INVALID_HOST_PTR";
    case CL_INVALID_MEM_OBJECT:
        return "CL_INVALID_MEM_OBJECT";
    case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
        return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
    case CL_INVALID_IMAGE_SIZE:
        return "CL_INVALID_IMAGE_SIZE";
    case CL_INVALID_SAMPLER:
        return "CL_INVALID_SAMPLER";
    case CL_INVALID_BINARY:
        return "CL_INVALID_BINARY";
    case CL_INVALID_BUILD_OPTIONS:
        return "CL_INVALID_BUILD_OPTIONS";
    case CL_INVALID_PROGRAM:
        return "CL_INVALID_PROGRAM";
    case CL_INVALID_PROGRAM_EXECUTABLE:
        return "CL_INVALID_PROGRAM_EXECUTABLE";
    case CL_INVALID_KERNEL_NAME:
        return "CL_INVALID_KERNEL_NAME";
    case CL_INVALID_KERNEL_DEFINITION:
        return "CL_INVALID_KERNEL_DEFINITION";
    case CL_INVALID_KERNEL:
        return "CL_INVALID_KERNEL";
    case CL_INVALID_ARG_INDEX:
        return "CL_INVALID_ARG_INDEX";
    case CL_INVALID_ARG_VALUE:
        return "CL_INVALID_ARG_VALUE";
    case CL_INVALID_ARG_SIZE:
        return "CL_INVALID_ARG_SIZE";
    case CL_INVALID_KERNEL_ARGS:
        return "CL_INVALID_KERNEL_ARGS";
    case CL_INVALID_WORK_DIMENSION:
        return "CL_INVALID_WORK_DIMENSION";
    case CL_INVALID_WORK_GROUP_SIZE:
        return "CL_INVALID_WORK_GROUP_SIZE";
    case CL_INVALID_WORK_ITEM_SIZE:
        return "CL_INVALID_WORK_ITEM_SIZE";
    case CL_INVALID_GLOBAL_OFFSET:
        return "CL_INVALID_GLOBAL_OFFSET";
    case CL_INVALID_EVENT_WAIT_LIST:
        return "CL_INVALID_EVENT_WAIT_LIST";
    case CL_INVALID_EVENT:
        return "CL_INVALID_EVENT";
    case CL_INVALID_OPERATION:
        return "CL_INVALID_OPERATION";
    case CL_INVALID_GL_OBJECT:
        return "CL_INVALID_GL_OBJECT";
    case CL_INVALID_BUFFER_SIZE:
        return "CL_INVALID_BUFFER_SIZE";
    case CL_INVALID_MIP_LEVEL:
        return "CL_INVALID_MIP_LEVEL";
    case CL_INVALID_GLOBAL_WORK_SIZE:
        return "CL_INVALID_GLOBAL_WORK_SIZE";
    case CL_INVALID_PROPERTY:
        return "CL_INVALID_PROPERTY";

#ifdef CL_VERSION_1_2
    case CL_INVALID_IMAGE_DESCRIPTOR:
        return "CL_INVALID_IMAGE_DESCRIPTOR";
    case CL_INVALID_COMPILER_OPTIONS:
        return "CL_INVALID_COMPILER_OPTIONS";
    case CL_INVALID_LINKER_OPTIONS:
        return "CL_INVALID_LINKER_OPTIONS";
    case CL_INVALID_DEVICE_PARTITION_COUNT:
        return "CL_INVALID_DEVICE_PARTITION_COUNT";
#endif // CL_VERSION_1_2

#ifdef CL_VERSION_2_0
    case CL_INVALID_PIPE_SIZE:
        return "CL_INVALID_PIPE_SIZE";
    case CL_INVALID_DEVICE_QUEUE:
        return "CL_INVALID_DEVICE_QUEUE";
#endif // CL_VERSION_2_0

#ifdef CL_VERSION_2_2
    case CL_INVALID_SPEC_ID:
        return "CL_INVALID_SPEC_ID";
    case CL_MAX_SIZE_RESTRICTION_EXCEEDED:
        return "CL_MAX_SIZE_RESTRICTION_EXCEEDED";
#endif // CL_VERSION_2_2
    }

    return "Unknown CL error encountered";
}

/**
 * Prints cl::Errors in a uniform way
 * @param msg text prepending the error message
 * @param clerr cl:Error object
 *
 * Prints errors in the format:
 *      msg: what(), string err() (numeric err())
 */
static string ethCLErrorHelper(const char* msg, cl::Error const& clerr) {
    ostringstream osstream;
    osstream << msg << ": " << clerr.what() << ": " << strClError(clerr.err()) << " (" << clerr.err() << ")";
    return osstream.str();
}

namespace {
void addDefinition(string& _source, char const* _id, unsigned _value) {
    char buf[256];
    sprintf(buf, "#define %s %uu\n", _id, _value);
    _source.insert(_source.begin(), buf, buf + strlen(buf));
}

vector<cl::Platform> getPlatforms() {
    vector<cl::Platform> platforms;
    try {
        cl::Platform::get(&platforms);
    } catch (cl::Error const& err) {
#if defined(CL_PLATFORM_NOT_FOUND_KHR)
        if (err.err() == CL_PLATFORM_NOT_FOUND_KHR)
            cerr << "No OpenCL platforms found" << endl;
        else
#endif
            cerr << "OpenCL error : " << err.what();
    }
    return platforms;
}

vector<cl::Device> getDevices(vector<cl::Platform> const& _platforms, unsigned _platformId) {
    vector<cl::Device> devices;
    size_t platform_num = min<size_t>(_platformId, _platforms.size() - 1);
    try {
        _platforms[platform_num].getDevices(CL_DEVICE_TYPE_GPU | CL_DEVICE_TYPE_ACCELERATOR, &devices);
    } catch (cl::Error const& err) {
        // if simply no devices found return empty vector
        if (err.err() != CL_DEVICE_NOT_FOUND)
            throw err;
    }
    return devices;
}

} // namespace

} // namespace exp
} // namespace dev

CLMiner::CLMiner(unsigned _index, DeviceDescriptor& _device) : Miner("cl-", _index) {
    m_deviceDescriptor = _device;
    m_block_multiple = 200000;
}

CLMiner::~CLMiner() {
    stopWorking();
    kick_miner();
}

// NOTE: The following struct must match the one defined in
// frkhash.cl

struct SearchResults {
    struct
    {
        uint32_t gid;
        //uint64_t sol_targ;
        //uint64_t sol_hea;
    } rslt[c_maxSearchResults];
    uint32_t count;
    uint32_t hashCount;
    uint32_t abort;
};

//const static uint32_t zerox3[3] = {0, 0, 0};

void CLMiner::workLoop() {
    // Memory for zero-ing buffers. Cannot be static or const because crashes on macOS.
    uint32_t zerox3[3] = {0, 0, 0};

    uint64_t startNonce = 0;

    // The work package currently processed by GPU.
    WorkPackage current;
    current.header = h256();
    
    if (!initDevice())
        return;

    try {
        while (!shouldStop()) {
	  for (uint32_t j = 0; j < nStreams; j++) {
            // Read results.
            SearchResults results;

            if (m_queue.size())
            {
                // no need to read the abort flag.
                m_queue[j].enqueueReadBuffer(m_searchBuffer[j], CL_TRUE,
                    offsetof(SearchResults, count), 2 * sizeof(results.count), (void*)&results.count);
                if (results.count)
                {
                    if (results.count > c_maxSearchResults) {
                        results.count = c_maxSearchResults;
                    }
                    cnote << "RECEIVED RESULTS: " << results.count;
                    m_queue[j].enqueueReadBuffer(m_searchBuffer[j], CL_TRUE, 0, results.count * sizeof(results.rslt[0]), (void*)&results);

                    // Reset search count if any solution found.
                    m_queue[j].enqueueWriteBuffer(m_searchBuffer[j], CL_FALSE, offsetof(SearchResults, count), sizeof(results.count), zerox3);                    
                }
                // clean the solution hash count
		m_queue[j].enqueueWriteBuffer(m_searchBuffer[j], CL_FALSE, offsetof(SearchResults, hashCount), sizeof(results.hashCount), zerox3);
            }
            else
                results.count = 0;

            // Wait for work or 3 seconds (whichever the first)
            WorkPackage w = work();
            if (!w) {
                m_hung_miner.store(false);
                unique_lock<mutex> l(miner_work_mutex);
                m_new_work_signal.wait_for(l, chrono::seconds(3));
                continue;
            }


            if (current.header != w.header) current = w;
            m_abortqueue.clear();

            if (!initEpoch_frk(j))
                    break;  // This will simply exit the thread

            m_abortqueue.push_back(cl::CommandQueue(m_context[0], m_device));

            w = current;

            startNonce = startNonce ? startNonce : w.startNonce;

            // Update header
            m_queue[j].enqueueWriteBuffer(m_header[j], CL_FALSE, 0, w.header.size, w.header.data());

            // zero the result count
            m_queue[j].enqueueWriteBuffer(m_searchBuffer[j], CL_FALSE, offsetof(SearchResults, count), sizeof(zerox3), zerox3);
            // Upper 64 bits of the boundary.
            const uint64_t target = (uint64_t)(u64)((u256)w.boundary >> 192);
            assert(target > 0);
		
            m_searchKernel.setArg(1, m_header[j]);
            m_searchKernel.setArg(2, startNonce);
            m_searchKernel.setArg(3, target);
#ifdef DEV_BUILD
            if (g_logOptions & LOG_SWITCH)
                    cnote << "Switch time: "
                          << chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() -
                                                                         m_workSwitchStart)
                                 .count()
                          << " us.";
#endif


            float hr = RetrieveHashRate();
#ifdef CL_TARGET_BATCH_TIME_DOUBLE
            if (hr > 1e7)
                m_block_multiple = uint32_t(hr * CL_TARGET_BATCH_TIME_DOUBLE / m_deviceDescriptor.clGroupSize);
#else
            if (hr > 1e7)
                m_block_multiple = uint32_t(hr * CL_TARGET_BATCH_TIME / m_deviceDescriptor.clGroupSize);
#endif
            uint32_t batch_blocks = m_deviceDescriptor.clGroupSize * m_block_multiple;

            // Run the kernel.

            m_hung_miner.store(false);
            m_queue[j].enqueueNDRangeKernel(m_searchKernel, cl::NullRange, batch_blocks, m_deviceDescriptor.clGroupSize);

            // Report results while the kernel is running.
            if (results.count > c_maxSearchResults)
                results.count = c_maxSearchResults;
            for (uint32_t i = 0; i < results.count; i++) {
		uint64_t nonce = current.startNonce + results.rslt[i].gid;

		if (nonce != m_lastNonce)
		{
			m_lastNonce = nonce;
			Farm::f().submitProof(Solution{nonce, h256(), current, chrono::steady_clock::now(), m_index});
			ReportSolution(current.header, nonce);
		}

            }
            
            current = w; // kernel now processing newest work
            current.startNonce = startNonce;
            // Increase start nonce for following kernel execution.
            startNonce += batch_blocks;
            // Report hash count
            updateHashRate(m_deviceDescriptor.clGroupSize, results.hashCount);
	  } //for
        } // while

        if (m_queue.size()) for (uint32_t j = 0; j < nStreams; j++) m_queue[j].finish();

        free_buffers();
        m_abortMutex.unlock();
    } catch (cl::Error const& _e) {
        string _what = ethCLErrorHelper("OpenCL Error", _e);
        free_buffers();
        m_abortMutex.unlock();
        throw runtime_error(_what);
    }
}

void CLMiner::kick_miner() {
    m_abortMutex.lock();
    // Memory for abort Cannot be static because crashes on macOS.
    if (!m_abortqueue.empty()) {
        static uint32_t one = 1;
        for (uint32_t j = 0; j < nStreams; j++) m_abortqueue[j].enqueueWriteBuffer(m_searchBuffer[j], CL_FALSE, offsetof(SearchResults, abort), sizeof(one), &one);
    }
    m_abortMutex.unlock();
    m_new_work_signal.notify_one();
}

void CLMiner::enumDevices(minerMap& _DevicesCollection) {
    // Load available platforms
    vector<cl::Platform> platforms = getPlatforms();
    if (platforms.empty())
        return;

    unsigned int dIdx = 0;
    for (unsigned int pIdx = 0; pIdx < platforms.size(); pIdx++) {
        string platformName = platforms.at(pIdx).getInfo<CL_PLATFORM_NAME>();
        ClPlatformTypeEnum platformType = ClPlatformTypeEnum::Unknown;
        if (platformName == "AMD Accelerated Parallel Processing")
            platformType = ClPlatformTypeEnum::Amd;
        else if (platformName == "Clover" || platformName == "Intel Gen OCL Driver")
            platformType = ClPlatformTypeEnum::Clover;
        else if (platformName == "NVIDIA CUDA")
            platformType = ClPlatformTypeEnum::Nvidia;
        else if (platformName.find("Intel") != string::npos)
            platformType = ClPlatformTypeEnum::Intel;
        else
            continue;

        string platformVersion = platforms.at(pIdx).getInfo<CL_PLATFORM_VERSION>();
        unsigned int platformVersionMajor = stoi(platformVersion.substr(7, 1));
        unsigned int platformVersionMinor = stoi(platformVersion.substr(9, 1));

        dIdx = 0;
        vector<cl::Device> devices = getDevices(platforms, pIdx);
        for (auto const& device : devices) {
            DeviceTypeEnum clDeviceType = DeviceTypeEnum::Unknown;
            cl_device_type detectedType = device.getInfo<CL_DEVICE_TYPE>();
            if (detectedType == CL_DEVICE_TYPE_GPU)
                clDeviceType = DeviceTypeEnum::Gpu;
            else if (detectedType == CL_DEVICE_TYPE_CPU)
                clDeviceType = DeviceTypeEnum::Cpu;
            else if (detectedType == CL_DEVICE_TYPE_ACCELERATOR)
                clDeviceType = DeviceTypeEnum::Accelerator;
            else
                continue;

            string uniqueId;
            DeviceDescriptor deviceDescriptor;

            if (clDeviceType == DeviceTypeEnum::Gpu && platformType == ClPlatformTypeEnum::Nvidia) {
                cl_int bus_id, slot_id;
                if (clGetDeviceInfo(device.get(), 0x4008 /*CL_DEVICE_PCI_BUS_ID_NV*/, sizeof(bus_id), &bus_id, NULL) ==
                        CL_SUCCESS &&
                    clGetDeviceInfo(device.get(), 0x4009 /*CL_DEVICE_PCI_SLOT_ID_NV*/, sizeof(slot_id), &slot_id,
                                    NULL) == CL_SUCCESS) {
                    ostringstream s;
                    s << "0000:" << setfill('0') << setw(2) << hex << bus_id << ":" << setw(2)
                      << (unsigned int)(slot_id >> 3) << "." << (unsigned int)(slot_id & 0x7);
                    uniqueId = s.str();
                }
            } else if (clDeviceType == DeviceTypeEnum::Gpu &&
                       (platformType == ClPlatformTypeEnum::Amd || platformType == ClPlatformTypeEnum::Clover)) {
                cl_char t[24];
                if (clGetDeviceInfo(device.get(), 0x4037 /*CL_DEVICE_TOPOLOGY_AMD*/, sizeof(t), &t, NULL) ==
                    CL_SUCCESS) {
                    // NOTE" Till we can upgrade to opencl 2.x, there's no way to determine
                    // the bus domain id. So we plug in a 0!
                    ostringstream s;
                    s << "0000:" << setfill('0') << setw(2) << hex << (unsigned int)(t[21]) << ":" << setw(2)
                      << (unsigned int)(t[22]) << "." << (unsigned int)(t[23]);
                    uniqueId = s.str();
                }
            } else if (clDeviceType == DeviceTypeEnum::Gpu && platformType == ClPlatformTypeEnum::Intel) {
                ostringstream s;
                s << "Intel GPU " << pIdx << "." << dIdx;
                uniqueId = s.str();
            } else if (clDeviceType == DeviceTypeEnum::Cpu) {
                ostringstream s;
                s << "CPU " << setfill('0') << setw(2) << hex << (pIdx + dIdx);
                uniqueId = s.str();
            } else {
                // We're not prepared (yet) to handle other platforms or types
                ++dIdx;
                continue;
            }

            if (_DevicesCollection.find(uniqueId) != _DevicesCollection.end())
                deviceDescriptor = _DevicesCollection[uniqueId];
            else
                deviceDescriptor = DeviceDescriptor();

            // Fill the blanks by OpenCL means
            deviceDescriptor.type = clDeviceType;
            deviceDescriptor.uniqueId = uniqueId;
            deviceDescriptor.clDetected = true;
            deviceDescriptor.clPlatformId = pIdx;
            deviceDescriptor.clPlatformName = platformName;
            deviceDescriptor.clPlatformType = platformType;
            deviceDescriptor.clPlatformVersion = platformVersion;
            deviceDescriptor.clPlatformVersionMajor = platformVersionMajor;
            deviceDescriptor.clPlatformVersionMinor = platformVersionMinor;
            deviceDescriptor.clDeviceOrdinal = dIdx;
            deviceDescriptor.clDeviceVersion = device.getInfo<CL_DEVICE_VERSION>();
            deviceDescriptor.clDeviceVersionMajor = stoi(deviceDescriptor.clDeviceVersion.substr(7, 1));
            deviceDescriptor.clDeviceVersionMinor = stoi(deviceDescriptor.clDeviceVersion.substr(9, 1));
            deviceDescriptor.totalMemory = device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>();
            deviceDescriptor.clGroupSize = 64;

            // Is it an NVIDIA card ?
            if (platformType == ClPlatformTypeEnum::Nvidia) {
                size_t siz;
                clGetDeviceInfo(device.get(), CL_DEVICE_COMPUTE_CAPABILITY_MAJOR_NV,
                                sizeof(deviceDescriptor.clNvComputeMajor), &deviceDescriptor.clNvComputeMajor, &siz);
                clGetDeviceInfo(device.get(), CL_DEVICE_COMPUTE_CAPABILITY_MINOR_NV,
                                sizeof(deviceDescriptor.clNvComputeMinor), &deviceDescriptor.clNvComputeMinor, &siz);
                deviceDescriptor.clNvCompute =
                    to_string(deviceDescriptor.clNvComputeMajor) + "." + to_string(deviceDescriptor.clNvComputeMinor);
                deviceDescriptor.boardName = device.getInfo<CL_DEVICE_NAME>();
            }
            // AMD GPU
            else {
                deviceDescriptor.clArch = device.getInfo<CL_DEVICE_NAME>();
                size_t s1;
                char s[256];
#define CL_DEVICE_BOARD_NAME_AMD 0x4038
                clGetDeviceInfo(device.get(), CL_DEVICE_BOARD_NAME_AMD, sizeof(s), s, &s1);
                deviceDescriptor.boardName = s;
            }

            // Upsert Devices Collection
            _DevicesCollection[uniqueId] = deviceDescriptor;
            ++dIdx;
        }
    }
}

bool CLMiner::initDevice() {
    m_initialized = false;
    // LookUp device
    // Load available platforms
    vector<cl::Platform> platforms = getPlatforms();
    if (platforms.empty())
        return false;

    vector<cl::Device> devices = getDevices(platforms, m_deviceDescriptor.clPlatformId);
    if (devices.empty())
        return false;

    m_device = devices.at(m_deviceDescriptor.clDeviceOrdinal);

    // Set Hardware Monitor Info
    if (m_deviceDescriptor.clPlatformType == ClPlatformTypeEnum::Nvidia) {
        m_hwmoninfo.deviceType = HwMonitorInfoType::NVIDIA;
        m_hwmoninfo.devicePciId = m_deviceDescriptor.uniqueId;
        m_hwmoninfo.deviceIndex = -1; // Will be later on mapped by nvml (see Farm() constructor)
    } else if (m_deviceDescriptor.clPlatformType == ClPlatformTypeEnum::Amd) {
        m_hwmoninfo.deviceType = HwMonitorInfoType::AMD;
        m_hwmoninfo.devicePciId = m_deviceDescriptor.uniqueId;
        m_hwmoninfo.deviceIndex = -1; // Will be later on mapped by nvml (see Farm() constructor)
    } else if (m_deviceDescriptor.clPlatformType == ClPlatformTypeEnum::Clover) {
        m_hwmoninfo.deviceType = HwMonitorInfoType::UNKNOWN;
        m_hwmoninfo.devicePciId = m_deviceDescriptor.uniqueId;
        m_hwmoninfo.deviceIndex = -1; // Will be later on mapped by nvml (see Farm() constructor)
    } else if (m_deviceDescriptor.clPlatformType == ClPlatformTypeEnum::Intel) {
        m_hwmoninfo.deviceType = HwMonitorInfoType::UNKNOWN;
        m_hwmoninfo.devicePciId = m_deviceDescriptor.uniqueId;
        m_hwmoninfo.deviceIndex = -1; // Will be later on mapped by nvml (see Farm() constructor)
    } else {
        // Don't know what to do with this
        cwarn << "Unrecognized Platform";
        return false;
    }

    if (m_deviceDescriptor.clPlatformVersionMajor == 1 &&
        (m_deviceDescriptor.clPlatformVersionMinor == 0 || m_deviceDescriptor.clPlatformVersionMinor == 1)) {
        if (m_deviceDescriptor.clPlatformType == ClPlatformTypeEnum::Clover) {
            cwarn << "OpenCL " << m_deviceDescriptor.clPlatformVersion
                  << " not supported, but platform Clover might work nevertheless. USE AT OWN RISK!";
        } else {
            cwarn << "OpenCL " << m_deviceDescriptor.clPlatformVersion
                  << " not supported. Minimum required version is 1.2";
            throw new runtime_error("OpenCL 1.2 required");
        }
    }

    ostringstream s;
    s << "Using Pci " << m_deviceDescriptor.uniqueId << ": " << m_deviceDescriptor.boardName;

    if (!m_deviceDescriptor.clNvCompute.empty())
        s << " (Compute " + m_deviceDescriptor.clNvCompute + ")";
    else
        s << " (" << m_deviceDescriptor.clDeviceVersion;

    s << ") Memory : " << dev::getFormattedMemory((double)m_deviceDescriptor.totalMemory);
    cextr << s.str();

    return true;
}

bool CLMiner::initEpoch() { return true; }

bool CLMiner::initEpoch_frk(uint32_t jj) {
    m_initialized = false;
    uint32_t zerox3[3] = {0, 0, 0};

    try {
        char options[256] = {0};
        int computeCapability = 0;

        // Nvidia
        if (!m_deviceDescriptor.clNvCompute.empty()) {
            computeCapability = m_deviceDescriptor.clNvComputeMajor * 10 + m_deviceDescriptor.clNvComputeMinor;
            int maxregs = computeCapability >= 35 ? 72 : 63;
            sprintf(options, "-cl-nv-maxrregcount=%d", maxregs);
        }

        free_buffers();
        
	    // create context

        m_context.clear();
        m_context.push_back(cl::Context(vector<cl::Device>(&m_device, &m_device + 1)));
        m_queue.clear();
        m_queue.push_back(cl::CommandQueue(m_context[0], m_device));
	
	    m_searchBuffer.clear();
	    m_searchBuffer.emplace_back(m_context[0], CL_MEM_WRITE_ONLY, sizeof(SearchResults));

        // Release the pause flag if any
        resume(MinerPauseEnum::PauseDueToInsufficientMemory);
        resume(MinerPauseEnum::PauseDueToInitEpochError);

        // patch source code
        // note: The kernels here are simply compiled version of the respective .cl kernels
        // into a byte array by bin2h.cmake. There is no need to load the file by hand in runtime
        // See libcl/CMakeLists.txt: add_custom_command()
        // TODO: Just use C++ raw string literal.
        string code;

        code = string(frkhash_cl, frkhash_cl + sizeof(frkhash_cl));

        addDefinition(code, "WORKSIZE", m_deviceDescriptor.clGroupSize);
        addDefinition(code, "ACCESSES", 64);
        addDefinition(code, "MAX_OUTPUTS", c_maxSearchResults);
        addDefinition(code, "PLATFORM", static_cast<unsigned>(m_deviceDescriptor.clPlatformType));
        addDefinition(code, "COMPUTE", computeCapability);

        // create miner OpenCL program
        cl::Program::Sources sources{{code.data(), code.size()}};
        cl::Program program(m_context[0], sources);
        try {
            program.build({m_device}, options);
        } catch (cl::BuildError const& buildErr) {
            ccrit << "OpenCL kernel build log:\n" << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(m_device);
            ccrit << "OpenCL kernel build error (" << buildErr.err() << "):\n" << buildErr.what();
            pause(MinerPauseEnum::PauseDueToInitEpochError);
            free_buffers();
            return false;
        }

        try {
            m_searchKernel = cl::Kernel(program, "search");
        } catch (cl::Error const& err) {
            cwarn << ethCLErrorHelper("Creating opencl failed", err);
            pause(MinerPauseEnum::PauseDueToInitEpochError);
            free_buffers();
            return false;
        }
        // create buffer for header

        m_header.clear();
        m_header.push_back(cl::Buffer(m_context[0], CL_MEM_READ_ONLY, 32));

        // for (uint32_t j = 0; j < nStreams; j++) m_queue[j].enqueueWriteBuffer(m_searchBuffer[j], CL_FALSE, 0, sizeof(zerox3), zerox3);
	    m_queue[jj].enqueueWriteBuffer(m_searchBuffer[jj], CL_FALSE, 0, sizeof(zerox3), zerox3);

        m_searchKernel.setArg(0, m_searchBuffer[jj]); // Supply output buffer to kernel.
    } catch (cl::Error const& err) {
        ccrit << ethCLErrorHelper("OpenCL init failed", err);
        pause(MinerPauseEnum::PauseDueToInitEpochError);
        free_buffers();
        return false;
    }
    m_initialized = true;
    m_abortMutex.unlock();
    return true;
}
