#pragma once
#include "core/bse_core.h"

struct AppData
{
  //your hot-reload persistent data goes here
};

//this will be set for you every time 
extern AppData* appData;


struct Clock
{
  u64 nanoseconds_startTime;
  u64 nanoseconds_elapsed;
};

//get_absolute_time() from platform
void clock_update( Clock* clock ) {}
void clock_start( Clock* clock ) {}
void clock_stop( Clock* clock ) {}







/*
Tests that need to be done:
TODO

leading_zeros(u16,u32,u64);
sleep
is_current_thread
get_current_thread_id
interlocked_compare_exchange
interlocked_increment
interlocked_decrement



*/

//__ANDROID__
//__ARM_ARCH
//__ARM_32_BIT_STATE ??


#define BSE_VK_USE_CUSTOM_ALLOCATOR

namespace bse
{
  struct QueueInfo
  {
    QueueInfo()
      : graphicsFamilyIndex( VK_QUEUE_FAMILY_IGNORED )
      , presentFamilyIndex( VK_QUEUE_FAMILY_IGNORED )
      , computeFamilyIndex( VK_QUEUE_FAMILY_IGNORED )
      , transferFamilyIndex( VK_QUEUE_FAMILY_IGNORED ) {}
    u32 graphicsFamilyIndex;
    u32 presentFamilyIndex;
    u32 computeFamilyIndex;
    u32 transferFamilyIndex;
  };

  struct SwapchainSupportInfo
  {
    bse::Vector<VkSurfaceFormatKHR> surfaceFormats;
    bse::Vector<VkPresentModeKHR> presentModes;
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
  };

  struct VulkanDevice
  {
    VkPhysicalDevice physicalDevice;
    VkDevice logicalDevice;
    QueueInfo queueInfo;
    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;
    VkPhysicalDeviceMemoryProperties memoryProperties;
    SwapchainSupportInfo swapchainSupportInfo;

    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkQueue transferQueue;
  };

  struct VulkanPhysicalDeviceRequirements
  {
    QueueInfo queueInfo;
    VkPhysicalDeviceType deviceType;
    bse::Vector<bse::String> deviceExtensionNames;
  };

  struct VulkanAllocationUserData
  {
    bse::Vector<void*> allocations;
    bse::Vector<s64> sizes;

    void* allocate( s64 size, s64 alignment );
    void free( void* allocation );
    void* reallocate( void* allocation, s64 newSize, s64 alignment );
  };

  struct VulkanContext
  {
    VkInstance instance;
    VkAllocationCallbacks allocationCallbacks;
    #if defined(BSE_VK_USE_CUSTOM_ALLOCATOR)
    VulkanAllocationUserData allocationUserData;
    #endif
    VulkanDevice device;
  };

  QueueInfo get_queue_info( VkPhysicalDevice physicalDevice, VkSurfaceKHR surface );

  void* VKAPI_PTR BSE_vkAllocationFunction( void* pUserData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope )
  {
    #if defined(BSE_VK_USE_CUSTOM_ALLOCATOR)
    if ( allocationScope == VK_SYSTEM_ALLOCATION_SCOPE_COMMAND )
    {
      return bse::memory::allocate_temporary( size );
    }
    VulkanAllocationUserData* allocationUserData = (VulkanAllocationUserData*) pUserData;
    return allocationUserData->allocate( size, alignment );
    #else
    return malloc( size );
    #endif
  }

  void VKAPI_PTR BSE_vkFreeFunction( void* pUserData, void* pMemory )
  {
    #if defined(BSE_VK_USE_CUSTOM_ALLOCATOR)
    if ( !bse::memory::is_temporary( pMemory ) )
    {
      VulkanAllocationUserData* allocationUserData = (VulkanAllocationUserData*) pUserData;
      allocationUserData->free( pMemory );
    }
    #else
    free( pMemory );
    #endif
  }

  void VKAPI_PTR BSE_vkInternalAllocationNotification( void* pUserData, size_t size, VkInternalAllocationType allocationType, VkSystemAllocationScope allocationScope )
  {
    BREAK;
  }

  void VKAPI_PTR BSE_vkInternalFreeNotification( void* pUserData, size_t size, VkInternalAllocationType allocationType, VkSystemAllocationScope allocationScope )
  {
    BREAK;
  }

  void* VKAPI_PTR BSE_vkReallocationFunction( void* pUserData, void* pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope )
  {
    #if defined(BSE_VK_USE_CUSTOM_ALLOCATOR)
    VulkanAllocationUserData* allocationUserData = (VulkanAllocationUserData*) pUserData;
    return allocationUserData->reallocate( pOriginal, size, alignment );
    #else
    return realloc( pOriginal, size );
    #endif
  }

  void* VulkanAllocationUserData::allocate( s64 size, s64 alignment )
  {
    void* result = bse::memory::allocate_thread_safe( size );
    assert( (s64( result ) % alignment) == 0 );
    allocations.push_back( result );
    sizes.push_back( size );
    return result;
  }
  void VulkanAllocationUserData::free( void* allocation )
  {
    s64 count = allocations.size();
    for ( int i = s32( count - 1 ); i >= 0; --i )
    {
      if ( allocation == allocations[i] )
      {
        swap_to_end_and_pop( allocations, i );
        s64 size = swap_to_end_and_pop( sizes, i );
        bse::memory::free_thread_safe( allocation, size );
        break;
      }
    }
  }
  void* VulkanAllocationUserData::reallocate( void* allocation, s64 newSize, s64 alignment )
  {
    s64 count = allocations.size();
    for ( int i = s32( count - 1 ); i >= 0; --i )
    {
      if ( allocation == allocations[i] )
      {
        void* result = bse::memory::reallocate_thread_safe( allocations[i], sizes[i], newSize );
        assert( (s64( result ) % alignment) == 0 );
        allocations[i] = result;
        sizes[i] = newSize;
        return result;
      }
    }

    BREAK;
    return nullptr;
  }

  template<> s32 string_format_internal<VkPresentModeKHR>( char* destination, s32 capacity, VkPresentModeKHR value )
  {
    char const* text;
    if ( value == VK_PRESENT_MODE_IMMEDIATE_KHR )
      text = "VK_PRESENT_MODE_IMMEDIATE_KHR";
    else if ( value == VK_PRESENT_MODE_MAILBOX_KHR )
      text = "VK_PRESENT_MODE_MAILBOX_KHR";
    else if ( value == VK_PRESENT_MODE_FIFO_KHR )
      text = "VK_PRESENT_MODE_FIFO_KHR";
    else if ( value == VK_PRESENT_MODE_FIFO_RELAXED_KHR )
      text = "VK_PRESENT_MODE_FIFO_RELAXED_KHR";
    else if ( value == VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR )
      text = "VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR";
    else if ( value == VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR )
      text = "VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR";
    else if ( value == VK_PRESENT_MODE_MAX_ENUM_KHR )
      text = "VK_PRESENT_MODE_MAX_ENUM_KHR";
    else
      text = "Unknown";

    return string_format_internal( destination, capacity, text );
  }

  static VKAPI_ATTR VkBool32 VKAPI_CALL vulkan_debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
    void* userData )
  {
    if ( messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT )
    {
      log_warning( "[Vulkan] ", callbackData->pMessage );
    }
    else if ( messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT )
    {
      log_error( "[Vulkan] ", callbackData->pMessage );
    }
    else
    {
      log_info( "[Vulkan] ", callbackData->pMessage );
    }

    return VK_FALSE;
  }


  bool does_queue_info_meet_requirements( QueueInfo queueInfo, QueueInfo requirements )
  {
    return ((queueInfo.graphicsFamilyIndex != VK_QUEUE_FAMILY_IGNORED) || (requirements.graphicsFamilyIndex == VK_QUEUE_FAMILY_IGNORED)) &&
      ((queueInfo.presentFamilyIndex != VK_QUEUE_FAMILY_IGNORED) || (requirements.presentFamilyIndex == VK_QUEUE_FAMILY_IGNORED)) &&
      ((queueInfo.computeFamilyIndex != VK_QUEUE_FAMILY_IGNORED) || (requirements.computeFamilyIndex == VK_QUEUE_FAMILY_IGNORED)) &&
      ((queueInfo.transferFamilyIndex != VK_QUEUE_FAMILY_IGNORED) || (requirements.transferFamilyIndex == VK_QUEUE_FAMILY_IGNORED));
  }

  bool does_physical_device_meet_requirements( VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VulkanPhysicalDeviceRequirements requirements )
  {
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties( physicalDevice, &properties );
    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures( physicalDevice, &features );
    QueueInfo queueInfo = get_queue_info( physicalDevice, surface );

    if ( !does_queue_info_meet_requirements( queueInfo, requirements.queueInfo ) )
    {
      return false;
    }

    if ( requirements.deviceType != (properties.deviceType & requirements.deviceType) )
    {
      return false;
    }

    bse::Vector<VkExtensionProperties> availableExtensions;
    if ( requirements.deviceExtensionNames.size() )
    {
      u32 availableExtensionCount = 0;
      BSE_VK_CHECK( vkEnumerateDeviceExtensionProperties( physicalDevice, 0, &availableExtensionCount, 0 ) );
      availableExtensions.resize( availableExtensionCount );
      BSE_VK_CHECK( vkEnumerateDeviceExtensionProperties( physicalDevice, 0, &availableExtensionCount, availableExtensions.data() ) );
    }

    for ( auto requiredExtension : requirements.deviceExtensionNames )
    {
      bool found = false;
      for ( auto availableExtension : availableExtensions )
      {
        if ( bse::string_match( requiredExtension.c_str(), availableExtension.extensionName ) )
        {
          found = true;
          break;
        }
      }

      if ( !found )
      {
        log_warning( "Requested extension \"", requiredExtension.c_str(), "\" missing on \"", properties.deviceName, "\"." );
        break;
      }
    }

    return true;
  }

  VkResult create_debug_utils_messenger_ext( VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger ) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr( instance, "vkCreateDebugUtilsMessengerEXT" );
    if ( func != nullptr ) {
      return func( instance, pCreateInfo, pAllocator, pDebugMessenger );
    }
    else {
      return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
  }

  VkDebugUtilsMessengerEXT create_debug_messenger( VulkanContext* context )
  {
    VkDebugUtilsMessengerEXT result;
    u32 logSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;

    VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo = { VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT };
    debugUtilsMessengerCreateInfo.messageSeverity = logSeverity;
    debugUtilsMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debugUtilsMessengerCreateInfo.pfnUserCallback = vulkan_debug_callback;
    debugUtilsMessengerCreateInfo.pUserData = nullptr;
    BSE_VK_CHECK( create_debug_utils_messenger_ext( context->instance, &debugUtilsMessengerCreateInfo, &context->allocationCallbacks, &result ) );

    return result;
  }

  QueueInfo get_queue_info( VkPhysicalDevice physicalDevice, VkSurfaceKHR surface )
  {
    QueueInfo result {};

    u32 queueFamilyPropertyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties( physicalDevice, &queueFamilyPropertyCount, 0 );

    bse::Vector<VkQueueFamilyProperties> queueFamilyProperties { queueFamilyPropertyCount };
    vkGetPhysicalDeviceQueueFamilyProperties( physicalDevice, &queueFamilyPropertyCount, queueFamilyProperties.data() );

    s32 minTransferScore = 8192;
    for ( u32 i = 0; i < queueFamilyPropertyCount; ++i )
    {
      s32 transferScore = 0;

      if ( queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT )
      {
        result.graphicsFamilyIndex = i;
        ++transferScore;
      }
      if ( queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT )
      {
        result.computeFamilyIndex = i;
        ++transferScore;
      }
      if ( queueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT )
      {
        //Attempt to find a dedicated transfer queue, or the one with the least other things going on
        if ( transferScore < minTransferScore )
        {
          minTransferScore = transferScore;
          result.transferFamilyIndex = i;
        }
      }

      VkBool32 supportPresent = VK_FALSE;
      BSE_VK_CHECK( vkGetPhysicalDeviceSurfaceSupportKHR( physicalDevice, i, surface, &supportPresent ) );
      if ( supportPresent )
      {
        result.presentFamilyIndex = i;
      }
    }

    return result;
  }

  bool supports_presentation( VkPhysicalDevice physicalDevice, u32 familyIndex )
  {
    return bse::platform->vulkan_physical_device_supports_presentation( physicalDevice, familyIndex );
  }

  char const* get_physical_device_type_str( VkPhysicalDeviceProperties properties )
  {
    switch ( properties.deviceType )
    {
      case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
        return "VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU";
      case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
        return "VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU";
      case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
        return "VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU";
      case VK_PHYSICAL_DEVICE_TYPE_CPU:
        return "VK_PHYSICAL_DEVICE_TYPE_CPU";
      case VK_PHYSICAL_DEVICE_TYPE_OTHER:
      default:
        return "Unknown";
    }
  }

  bool fetch_physical_device( VulkanContext* context, VkSurfaceKHR surface, VulkanPhysicalDeviceRequirements requirements )
  {
    if ( !context || !context->instance )
    {
      log_error( "VulkanContext must hold valid VkInstance." );
      return false;
    }

    VkInstance instance = context->instance;
    VkPhysicalDevice physicalDevices[16];
    u32 physicalDeviceCount = array_count( physicalDevices );
    BSE_VK_CHECK( vkEnumeratePhysicalDevices( instance, &physicalDeviceCount, physicalDevices ) );

    VkPhysicalDevice discreteDevice = 0;
    VkPhysicalDevice fallbackDevice = 0;

    for ( u32 i = 0; i < physicalDeviceCount; ++i )
    {
      if ( !does_physical_device_meet_requirements( physicalDevices[i], surface, requirements ) )
      {
        continue;
      }

      VkPhysicalDeviceProperties properties;
      vkGetPhysicalDeviceProperties( physicalDevices[i], &properties );

      if ( !discreteDevice && properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU )
      {
        discreteDevice = physicalDevices[i];
      }

      if ( !fallbackDevice )
      {
        fallbackDevice = physicalDevices[i];
      }
    }

    VkPhysicalDevice result = discreteDevice ? discreteDevice : fallbackDevice;
    if ( result )
    {
      //set context members
      context->device.physicalDevice = result;
      context->device.queueInfo = get_queue_info( result, surface );
      context->device.properties = VkPhysicalDeviceProperties {};
      context->device.features = VkPhysicalDeviceFeatures {};
      context->device.memoryProperties = VkPhysicalDeviceMemoryProperties {};
      vkGetPhysicalDeviceProperties( result, &context->device.properties );
      vkGetPhysicalDeviceFeatures( result, &context->device.features );
      vkGetPhysicalDeviceMemoryProperties( result, &context->device.memoryProperties );

      VkPhysicalDeviceProperties& properties = context->device.properties;
      VkPhysicalDeviceMemoryProperties& memoryProperties = context->device.memoryProperties;
      log_info( "Selected GPU: \"", properties.deviceName, "\", Type: ", get_physical_device_type_str( properties ), ", Driver version: ", VK_VERSION_MAJOR( properties.driverVersion ), ".", VK_VERSION_MINOR( properties.driverVersion ), ".", VK_VERSION_PATCH( properties.driverVersion ), ", API version: ", VK_VERSION_MAJOR( properties.apiVersion ), ".", VK_VERSION_MINOR( properties.apiVersion ), ".", VK_VERSION_PATCH( properties.apiVersion ) );

      for ( u32 j = 0; j < memoryProperties.memoryHeapCount; ++j )
      {
        float gbs = float( memoryProperties.memoryHeaps[j].size ) / float( GigaBytes( 1 ) );
        if ( memoryProperties.memoryHeaps[j].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT )
        {
          log_info( "GPU local memory: ", gbs, " GB" );
        }
        else
        {
          log_info( "Shared system memory: ", gbs, " GB" );
        }
      }

      return true;
    }
    else
    {
      log_error( "No GPUs found" );
    }

    return false;
  }

  VkSurfaceKHR create_surface( VkInstance instance )
  {
    return bse::platform->vulkan_create_surface( instance );
  }

  bool create_instance( VulkanContext* context )
  {
    VkApplicationInfo appInfo = { VK_STRUCTURE_TYPE_APPLICATION_INFO };
    appInfo.apiVersion = VK_API_VERSION_1_2;
    appInfo.pApplicationName = "name goes here";
    appInfo.applicationVersion = VK_MAKE_VERSION( 1, 0, 0 );
    appInfo.pEngineName = "BSE";
    appInfo.engineVersion = VK_MAKE_VERSION( 1, 0, 0 );

    VkInstanceCreateInfo createInfo = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
    createInfo.pApplicationInfo = &appInfo;

    char const* extensions[] =
    {
      VK_KHR_SURFACE_EXTENSION_NAME,
      #if defined(BSE_PLATFORM_WINDOWS)
      "VK_KHR_win32_surface",
      #elif defined (BSE_PLATFORM_ANDROID)
      "VK_KHR_android_surface"
      #elif defined (BSE_PLATFORM_LINUX)
      "VK_KHR_xcb_surface"
      #endif
      #if defined(BSE_BUILD_DEBUG)
      VK_EXT_DEBUG_UTILS_EXTENSION_NAME
      #endif
    };
    createInfo.ppEnabledExtensionNames = extensions;
    createInfo.enabledExtensionCount = array_count( extensions );

    #if defined(BSE_BUILD_DEBUG)
    char const* debugLayers[] =
    {
      "VK_LAYER_KHRONOS_validation"
    };

    createInfo.ppEnabledLayerNames = debugLayers;
    createInfo.enabledLayerCount = array_count( debugLayers );

    u32 availableLayerCount = 0;
    BSE_VK_CHECK( vkEnumerateInstanceLayerProperties( &availableLayerCount, 0 ) );
    bse::Vector<VkLayerProperties> availableLayers { availableLayerCount };
    BSE_VK_CHECK( vkEnumerateInstanceLayerProperties( &availableLayerCount, availableLayers.data() ) );

    for ( auto expectedLayer : debugLayers )
    {
      bool found = false;
      for ( auto existingLayer : availableLayers )
      {
        if ( bse::string_match( existingLayer.layerName, expectedLayer ) )
        {
          found = true;
          break;
        }
      }
      if ( !found )
      {
        log_error( "Vulkan debug layer: ", expectedLayer, " not found." );
      }
    }

    #else
    createInfo.ppEnabledLayerNames = 0;
    createInfo.enabledLayerCount = 0;
    #endif

    BSE_VK_CHECK( vkCreateInstance( &createInfo, &context->allocationCallbacks, &context->instance ) );
    return context->instance != 0;
  }

  bool create_logical_device( VulkanContext* context )
  {
    if ( !context->device.physicalDevice )
    {
      log_error( "VulkanContext must hold valid VkPhysicalDevice, please call fetch_physical_device() before this." );
      return false;
    }

    bool presentSharesGraphicsQueue = context->device.queueInfo.graphicsFamilyIndex == context->device.queueInfo.presentFamilyIndex;
    bool transferSharesGraphicsQueue = context->device.queueInfo.graphicsFamilyIndex == context->device.queueInfo.transferFamilyIndex;
    bse::Vector<u32> indices;
    indices.reserve( 3 );
    indices.push_back( context->device.queueInfo.graphicsFamilyIndex );

    if ( !presentSharesGraphicsQueue )
      indices.push_back( context->device.queueInfo.presentFamilyIndex );
    if ( !transferSharesGraphicsQueue )
      indices.push_back( context->device.queueInfo.transferFamilyIndex );

    bse::Vector<VkDeviceQueueCreateInfo> queueCreateInfos { indices.size() };

    float queuePriorities[] = { 1.0f };
    for ( s32 i = 0; i < queueCreateInfos.size(); ++i )
    {
      queueCreateInfos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queueCreateInfos[i].pNext = 0;
      queueCreateInfos[i].flags = 0;
      queueCreateInfos[i].queueFamilyIndex = indices[i];
      queueCreateInfos[i].queueCount = (indices[i] == context->device.queueInfo.graphicsFamilyIndex) ? 2 : 1;
      queueCreateInfos[i].pQueuePriorities = queuePriorities;
    }

    VkPhysicalDeviceFeatures requestedFeatures = {};
    requestedFeatures.samplerAnisotropy = VK_TRUE;

    char const* extensions[] =
    {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    VkDeviceCreateInfo deviceCreateInfo = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
    deviceCreateInfo.queueCreateInfoCount = u32( queueCreateInfos.size() );
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceCreateInfo.enabledExtensionCount = array_count( extensions );
    deviceCreateInfo.ppEnabledExtensionNames = extensions;
    deviceCreateInfo.pEnabledFeatures = &requestedFeatures;

    context->device.logicalDevice = 0;
    BSE_VK_CHECK( vkCreateDevice( context->device.physicalDevice, &deviceCreateInfo, &context->allocationCallbacks, &context->device.logicalDevice ) );

    return context->device.logicalDevice != 0;
  }

  void query_swapchain_support( VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, SwapchainSupportInfo* out_swapchainSupportInfo )
  {
    if ( !out_swapchainSupportInfo )
    {
      log_warning( BSE_FUNCTION_NAME, "() - out_swapchainSupportInfo = nullptr" );
      return;
    }

    bse::Vector<VkSurfaceFormatKHR>& surfaceFormats = out_swapchainSupportInfo->surfaceFormats;
    bse::Vector<VkPresentModeKHR>& presentModes = out_swapchainSupportInfo->presentModes;
    u32 surfaceFormatCount = 0;
    u32 presentModeCount = 0;

    BSE_VK_CHECK( vkGetPhysicalDeviceSurfaceCapabilitiesKHR( physicalDevice, surface, &out_swapchainSupportInfo->surfaceCapabilities ) );

    BSE_VK_CHECK( vkGetPhysicalDeviceSurfaceFormatsKHR( physicalDevice, surface, &surfaceFormatCount, 0 ) );
    if ( surfaceFormatCount != 0 )
    {
      surfaceFormats.resize( surfaceFormatCount );
      BSE_VK_CHECK( vkGetPhysicalDeviceSurfaceFormatsKHR( physicalDevice, surface, &surfaceFormatCount, surfaceFormats.data() ) );
    }

    BSE_VK_CHECK( vkGetPhysicalDeviceSurfacePresentModesKHR( physicalDevice, surface, &presentModeCount, 0 ) );

    if ( presentModeCount != 0 )
    {
      presentModes.resize( presentModeCount );
      BSE_VK_CHECK( vkGetPhysicalDeviceSurfacePresentModesKHR( physicalDevice, surface, &presentModeCount, presentModes.data() ) );
    }
  }

  void destroy_device( VulkanContext* context )
  {
    context->device.graphicsQueue = 0;
    context->device.presentQueue = 0;
    context->device.transferQueue = 0;

    if ( context->device.logicalDevice )
    {
      vkDestroyDevice( context->device.logicalDevice, &context->allocationCallbacks );
      context->device.logicalDevice = 0;
      log_info( "Logical device destroyed." );
    }

    context->device.physicalDevice = 0;
    context->device.swapchainSupportInfo = SwapchainSupportInfo {};
  }

  void init( VulkanContext& context )
  {
    context = {};
    #if defined(BSE_VK_USE_CUSTOM_ALLOCATOR)
    context.allocationUserData.allocations.reserve( 1024 );
    context.allocationUserData.sizes.reserve( 1024 );
    context.allocationCallbacks.pUserData = &context.allocationUserData;
    #endif
    context.allocationCallbacks.pfnAllocation = BSE_vkAllocationFunction;
    context.allocationCallbacks.pfnReallocation = BSE_vkReallocationFunction;
    context.allocationCallbacks.pfnFree = BSE_vkFreeFunction;
    context.allocationCallbacks.pfnInternalAllocation = BSE_vkInternalAllocationNotification;
    context.allocationCallbacks.pfnInternalFree = BSE_vkInternalFreeNotification;

    if ( !create_instance( &context ) )
    {
      BREAK;
      return;
    }

    log_info( "VkInstance created" );

    #if defined(BSE_BUILD_DEBUG)
    VkDebugUtilsMessengerEXT debugMessenger = create_debug_messenger( &context );
    assert( debugMessenger );
    #endif

    //surface
    VkSurfaceKHR surface = create_surface( context.instance );

    //physical device
    VulkanPhysicalDeviceRequirements requirements;
    requirements.deviceType = VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
    requirements.queueInfo.graphicsFamilyIndex = 1;
    requirements.queueInfo.presentFamilyIndex = 1;
    requirements.queueInfo.computeFamilyIndex = 1;
    requirements.queueInfo.transferFamilyIndex = 1;

    if ( !fetch_physical_device( &context, surface, requirements ) )
    {
      BREAK;
      return;
    }
    bse::memory::clear_temporary();

    query_swapchain_support( context.device.physicalDevice, surface, &context.device.swapchainSupportInfo );
    log_info( "Available present modes: ", context.device.swapchainSupportInfo.presentModes );

    //device
    if ( !create_logical_device( &context ) )
    {
      BREAK;
      return;
    }

    log_info( "Logical device created." );

    vkGetDeviceQueue( context.device.logicalDevice, context.device.queueInfo.graphicsFamilyIndex, 0, &context.device.graphicsQueue );
    vkGetDeviceQueue( context.device.logicalDevice, context.device.queueInfo.presentFamilyIndex, 0, &context.device.presentQueue );
    vkGetDeviceQueue( context.device.logicalDevice, context.device.queueInfo.transferFamilyIndex, 0, &context.device.transferQueue );
    bse::memory::clear_temporary();





    destroy_device( &context );
    //swapchain

    //render pass

    //semaphores

    //queue

    //shaders

    //command pool



  }

};