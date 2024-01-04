#include "spark/render/vk/VulkanShaderProgram.h"
#include "spark/render/DescriptorSet.h"
#include "spark/render/vk/VulkanDescriptorSetLayout.h"
#include "spark/render/vk/VulkanPushConstantsLayout.h"
#include "spark/render/vk/VulkanPushConstantsRange.h"
#include "spark/render/vk/VulkanPipelineLayout.h"
#include "spark/render/vk/VulkanShaderModule.h"

#include "spark/base/Exception.h"

#include "spirv_reflect.h"

#include <format>
#include <numeric>
#include <ranges>

template <>
struct std::formatter<SpvReflectResult> : std::formatter<string_view>
{
    static auto format(const SpvReflectResult value, auto& ctx)
    {
        switch (value)
        {
        case SPV_REFLECT_RESULT_SUCCESS:
            return std::format_to(ctx.out(), "SPV_REFLECT_RESULT_SUCCESS");
        case SPV_REFLECT_RESULT_NOT_READY:
            return std::format_to(ctx.out(), "SPV_REFLECT_RESULT_NOT_READY");
        case SPV_REFLECT_RESULT_ERROR_PARSE_FAILED:
            return std::format_to(ctx.out(), "SPV_REFLECT_RESULT_ERROR_PARSE_FAILED");
        case SPV_REFLECT_RESULT_ERROR_ALLOC_FAILED:
            return std::format_to(ctx.out(), "SPV_REFLECT_RESULT_ERROR_ALLOC_FAILED");
        case SPV_REFLECT_RESULT_ERROR_RANGE_EXCEEDED:
            return std::format_to(ctx.out(), "SPV_REFLECT_RESULT_ERROR_RANGE_EXCEEDED");
        case SPV_REFLECT_RESULT_ERROR_NULL_POINTER:
            return std::format_to(ctx.out(), "SPV_REFLECT_RESULT_ERROR_NULL_POINTER");
        case SPV_REFLECT_RESULT_ERROR_INTERNAL_ERROR:
            return std::format_to(ctx.out(), "SPV_REFLECT_RESULT_ERROR_INTERNAL_ERROR");
        case SPV_REFLECT_RESULT_ERROR_COUNT_MISMATCH:
            return std::format_to(ctx.out(), "SPV_REFLECT_RESULT_ERROR_COUNT_MISMATCH");
        case SPV_REFLECT_RESULT_ERROR_ELEMENT_NOT_FOUND:
            return std::format_to(ctx.out(), "SPV_REFLECT_RESULT_ERROR_ELEMENT_NOT_FOUND");
        case SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_CODE_SIZE:
            return std::format_to(ctx.out(), "SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_CODE_SIZE");
        case SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_MAGIC_NUMBER:
            return std::format_to(ctx.out(), "SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_MAGIC_NUMBER");
        case SPV_REFLECT_RESULT_ERROR_SPIRV_UNEXPECTED_EOF:
            return std::format_to(ctx.out(), "SPV_REFLECT_RESULT_ERROR_SPIRV_UNEXPECTED_EOF");
        case SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_ID_REFERENCE:
            return std::format_to(ctx.out(), "SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_ID_REFERENCE");
        case SPV_REFLECT_RESULT_ERROR_SPIRV_SET_NUMBER_OVERFLOW:
            return std::format_to(ctx.out(), "SPV_REFLECT_RESULT_ERROR_SPIRV_SET_NUMBER_OVERFLOW");
        case SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_STORAGE_CLASS:
            return std::format_to(ctx.out(), "SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_STORAGE_CLASS");
        case SPV_REFLECT_RESULT_ERROR_SPIRV_RECURSION:
            return std::format_to(ctx.out(), "SPV_REFLECT_RESULT_ERROR_SPIRV_RECURSION");
        case SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_INSTRUCTION:
            return std::format_to(ctx.out(), "SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_INSTRUCTION");
        case SPV_REFLECT_RESULT_ERROR_SPIRV_UNEXPECTED_BLOCK_DATA:
            return std::format_to(ctx.out(), "SPV_REFLECT_RESULT_ERROR_SPIRV_UNEXPECTED_BLOCK_DATA");
        case SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_BLOCK_MEMBER_REFERENCE:
            return std::format_to(ctx.out(), "SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_BLOCK_MEMBER_REFERENCE");
        case SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_ENTRY_POINT:
            return std::format_to(ctx.out(), "SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_ENTRY_POINT");
        case SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_EXECUTION_MODE:
            return std::format_to(ctx.out(), "SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_EXECUTION_MODE");
        default:
            break;
        }

        return std::format_to(ctx.out(), "Unsupported SpvReflectResult value: {}", static_cast<int>(value));
    }
};

namespace spark::render::vk
{
    struct DescriptorInfo
    {
        unsigned int location;
        unsigned int elementSize;
        unsigned int elements;
        DescriptorType type;

        [[nodiscard]] constexpr bool operator==(const DescriptorInfo& other) const noexcept
        {
            return location == other.location && elementSize == other.elementSize && elements == other.elements && type == other.type;
        }

        [[nodiscard]] constexpr bool operator!=(const DescriptorInfo& other) const noexcept
        {
            return !(*this == other);
        }
    };

    struct DescriptorSetInfo
    {
        unsigned int space;
        ShaderStage stage;
        std::vector<DescriptorInfo> descriptors;
    };

    struct PushConstantRangeInfo
    {
    public:
        ShaderStage stage;
        unsigned int offset;
        unsigned int size;
    };

    struct VulkanShaderProgram::Impl
    {
        friend class VulkanShaderProgram;

    public:
        explicit Impl(const VulkanDevice& device, std::vector<std::unique_ptr<VulkanShaderModule>>&& modules)
            : m_device(device), m_shaders(std::move(modules)) {}

    private:
        const VulkanDevice& m_device;
        std::vector<std::unique_ptr<VulkanShaderModule>> m_shaders;
    };


    VulkanShaderProgram::VulkanShaderProgram(const VulkanDevice& device, std::vector<std::unique_ptr<VulkanShaderModule>>&& modules)
        : m_impl(std::make_unique<Impl>(device, std::move(modules))) {}

    VulkanShaderProgram::~VulkanShaderProgram() = default;

    std::vector<const VulkanShaderModule*> VulkanShaderProgram::shaders() const noexcept
    {
        std::vector<const VulkanShaderModule*> shaders;
        shaders.reserve(m_impl->m_shaders.size());
        std::ranges::transform(m_impl->m_shaders, std::back_inserter(shaders), [](const auto& shader) { return shader.get(); });
        return shaders;
    }

    std::shared_ptr<IPipelineLayout> VulkanShaderProgram::reflectPipelineLayout() const
    {
        std::unordered_map<unsigned int, DescriptorSetInfo> reflected_descriptor_sets_layouts;
        std::vector<PushConstantRangeInfo> reflected_push_constants_ranges;

        for (const auto& shader : m_impl->m_shaders)
        {
            const auto& byte_code = shader->byteCode();

            // Reflect the shader module
            spv_reflect::ShaderModule reflection(byte_code.size(), byte_code.c_str());
            SpvReflectResult result = reflection.GetResult();
            if (result != SPV_REFLECT_RESULT_SUCCESS)
                throw spark::base::UnknownException(std::format("Failed to reflect shader module {}", shader->fileName()));

            // Get the descriptor sets
            unsigned int reflected_descriptor_set_count = 0;
            if (result = reflection.EnumerateDescriptorSets(&reflected_descriptor_set_count, nullptr); result != SPV_REFLECT_RESULT_SUCCESS)
                throw spark::base::UnknownException(std::format("Failed to get descriptor sets count for shader module {}", shader->fileName()));

            std::vector<SpvReflectDescriptorSet*> reflected_descriptor_sets(reflected_descriptor_set_count);
            if (result = reflection.EnumerateDescriptorSets(&reflected_descriptor_set_count, reflected_descriptor_sets.data()); result != SPV_REFLECT_RESULT_SUCCESS)
                throw spark::base::UnknownException(std::format("Failed to enumerate descriptor sets for shader module {}", shader->fileName()));

            // Get the push constants
            unsigned int push_constant_count = 0;
            if ((result = reflection.EnumeratePushConstantBlocks(&push_constant_count, nullptr)) != SPV_REFLECT_RESULT_SUCCESS)
                throw spark::base::UnknownException(std::format("Unable to get push constants count (Error {0}).", result));

            std::vector<SpvReflectBlockVariable*> reflected_push_constants(push_constant_count);
            if ((result = reflection.EnumeratePushConstantBlocks(&push_constant_count, reflected_push_constants.data())) != SPV_REFLECT_RESULT_SUCCESS)
                throw spark::base::UnknownException(std::format("Unable to enumerate push constants (Error {0}).", result));

            // Parse the descriptor sets
            for (const auto* reflected_descriptor_set : reflected_descriptor_sets)
            {
                std::vector<DescriptorInfo> descriptors(reflected_descriptor_set->binding_count);
                std::ranges::generate(descriptors,
                                      [reflected_descriptor_set, i = 0]() mutable
                                      {
                                          const auto descriptor = reflected_descriptor_set->bindings[i++];

                                          // Filter the descriptor type
                                          std::optional<DescriptorType> descriptor_type;
                                          switch (descriptor->descriptor_type)
                                          {
                                          case SPV_REFLECT_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
                                              throw spark::base::NotImplementedException("Combined image samplers are not supported yet");
                                          case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
                                          case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:
                                              throw spark::base::NotImplementedException("Dynamic buffers are not supported yet");
                                          case SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLER:
                                              descriptor_type = DescriptorType::Sampler;
                                              break;
                                          case SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
                                              descriptor_type = DescriptorType::Texture;
                                              break;
                                          case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_IMAGE:
                                              descriptor_type = DescriptorType::RWTexture;
                                              break;
                                          case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
                                              descriptor_type = DescriptorType::Buffer;
                                              break;
                                          case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:
                                              descriptor_type = DescriptorType::RWBuffer;
                                              break;
                                          case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
                                              descriptor_type = DescriptorType::ConstantBuffer;
                                              break;
                                          case SPV_REFLECT_DESCRIPTOR_TYPE_INPUT_ATTACHMENT:
                                              descriptor_type = DescriptorType::InputAttachment;
                                              break;
                                          case SPV_REFLECT_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR:
                                              throw spark::base::NotImplementedException("Acceleration structures are not supported yet");
                                          case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER:
                                              /*
                                               * All buffers should have at least one member that stores the type info about the contained type. Descriptor arrays are of type `SpvOpTypeRuntimeArray`. To differentiate
                                               * between `ByteAddressBuffer` and `StructuredBuffer`, we check the type flags of the first member. If it identifies an array of DWORDs, we treat the descriptor as 
                                               * `ByteAddressBuffer`, though it could be a flavor of `StructuredBuffer<int>`. This is conceptually identical, so it ultimately makes no difference.
                                               */
                                              if (descriptor->type_description->op != SpvOp::SpvOpTypeRuntimeArray)
                                              {
                                                  if ((descriptor->type_description->members[0].type_flags & (SPV_REFLECT_TYPE_FLAG_ARRAY | SPV_REFLECT_TYPE_FLAG_INT)) != 0)
                                                      descriptor_type = descriptor->resource_type == SPV_REFLECT_RESOURCE_FLAG_SRV
                                                                            ? DescriptorType::ByteAddressBuffer
                                                                            : DescriptorType::RWByteAddressBuffer;
                                                  else
                                                      descriptor_type = descriptor->resource_type == SPV_REFLECT_RESOURCE_FLAG_SRV
                                                                            ? DescriptorType::StructuredBuffer
                                                                            : DescriptorType::RWStructuredBuffer;
                                              } else
                                              {
                                                  if ((descriptor->type_description->members[0].type_flags & (SPV_REFLECT_TYPE_FLAG_ARRAY | SPV_REFLECT_TYPE_FLAG_INT)) != 0)
                                                      descriptor_type = descriptor->resource_type == SPV_REFLECT_RESOURCE_FLAG_SRV
                                                                            ? DescriptorType::ByteAddressBuffer
                                                                            : DescriptorType::RWByteAddressBuffer;
                                                  else // Assume SPV_REFLECT_RESOURCE_FLAG_SRV resource type, since UAV arrays are not allowed.
                                                      descriptor_type = DescriptorType::StructuredBuffer;
                                              }
                                              break;
                                          }

                                          // Count the number of elements
                                          unsigned int descriptor_count = 1;

                                          if (descriptor->type_description->op == SpvOp::SpvOpTypeRuntimeArray)
                                              descriptor_count = std::numeric_limits<unsigned int>::max();
                                          else
                                              for (unsigned int j = 0; j < descriptor->array.dims_count; ++j)
                                                  descriptor_count *= descriptor->array.dims[j];

                                          // Create the descriptor layout info
                                          return DescriptorInfo {
                                              .location = descriptor->binding,
                                              .elementSize = descriptor->block.padded_size,
                                              .elements = descriptor_count,
                                              .type = descriptor_type.value(),
                                          };
                                      });

                if (!reflected_descriptor_sets_layouts.contains(reflected_descriptor_set->set))
                {
                    DescriptorSetInfo info {
                        .space = reflected_descriptor_set->set,
                        .stage = shader->stage(),
                        .descriptors = descriptors
                    };
                    reflected_descriptor_sets_layouts.insert(std::make_pair(reflected_descriptor_set->set, std::move(info)));
                } else
                {
                    // If the set already exists in another stage, merge it.
                    auto& layout = reflected_descriptor_sets_layouts[reflected_descriptor_set->set];

                    for (const auto& descriptor : descriptors)
                    {
                        // Add the descriptor, if no other descriptor has been bound to the location. Otherwise, check if the descriptors are equal and drop it.
                        const auto match = std::ranges::find_if(layout.descriptors,
                                                                [&descriptor](const DescriptorInfo& element)
                                                                {
                                                                    return element.location == descriptor.location;
                                                                });
                        if (match == layout.descriptors.end())
                            layout.descriptors.push_back(descriptor);
                        else if (descriptor != *match)
                            log::warning("Mismatching descriptors detected: the descriptor at location {0} ({3} elements with size of {4} bytes) of the descriptor set {1} in shader stage {2} conflicts with a descriptor from at least one other shader stage and will be dropped (conflicts with descriptor of type {9} in stage/s {6} with {7} elements of {8} bytes).",
                                         descriptor.location,
                                         reflected_descriptor_set->set,
                                         shader->stage(),
                                         descriptor.elements,
                                         descriptor.elementSize,
                                         layout.stage,
                                         match->elements,
                                         match->elementSize,
                                         match->type);
                    }

                    // Store the stage.
                    layout.stage = shader->stage() | layout.stage;
                }
            }

            // Parse push constants
            if (push_constant_count > 1)
                log::info("More than one push constant range detected for shader stage {}. If you have multiple entry points, you may be able to split them up into different shader files",
                          shader->stage());

            for (const auto* reflected_push_constant : reflected_push_constants)
            {
                PushConstantRangeInfo info {
                    .stage = shader->stage(),
                    .offset = reflected_push_constant->absolute_offset,
                    .size = reflected_push_constant->padded_size,
                };
                reflected_push_constants_ranges.push_back(info);
            }
        }

        // Create the descriptor set layouts.
        std::vector<std::unique_ptr<VulkanDescriptorSetLayout>> descriptor_sets_layouts;
        for (auto& reflected_descriptor_info : reflected_descriptor_sets_layouts | std::views::values)
        {
            // Create the descriptor layouts.
            std::vector<std::unique_ptr<VulkanDescriptorLayout>> descriptor_layouts;
            for (const auto& reflected_descriptor : reflected_descriptor_info.descriptors)
                descriptor_layouts.push_back(std::make_unique<VulkanDescriptorLayout>(reflected_descriptor.type,
                                                                                      reflected_descriptor.location,
                                                                                      reflected_descriptor.elementSize,
                                                                                      reflected_descriptor.elements));
            descriptor_sets_layouts.push_back(std::make_unique<VulkanDescriptorSetLayout>(m_impl->m_device,
                                                                                          std::move(descriptor_layouts),
                                                                                          reflected_descriptor_info.space,
                                                                                          reflected_descriptor_info.stage));
        }

        // Create the push constants layout.
        std::vector<std::unique_ptr<VulkanPushConstantsRange>> push_constants_layouts(reflected_push_constants_ranges.size());
        for (auto& reflected_push_constant_range : reflected_push_constants_ranges)
        {
            push_constants_layouts.push_back(std::make_unique<VulkanPushConstantsRange>(reflected_push_constant_range.stage,
                                                                                        reflected_push_constant_range.offset,
                                                                                        reflected_push_constant_range.size,
                                                                                        0,
                                                                                        0));
        }

        const unsigned int overall_size = std::accumulate(reflected_push_constants_ranges.begin(),
                                                          reflected_push_constants_ranges.end(),
                                                          0u,
                                                          [](unsigned int acc, const auto& range) { return acc + range.size; });
        auto push_constants_layout = std::make_unique<VulkanPushConstantsLayout>(std::move(push_constants_layouts), overall_size);

        // Create the pipeline layout and return it.
        return std::make_shared<VulkanPipelineLayout>(m_impl->m_device, std::move(descriptor_sets_layouts), std::move(push_constants_layout));
    }
}
