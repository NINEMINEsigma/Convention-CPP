#pragma once
#ifndef Convention_Runtime_String_Hpp
#define Convention_Runtime_String_Hpp

#include "Config.hpp"

namespace Convention
{
	/**
	 * @brief 限制字符串长度，如果超过最大长度则截取头尾部分并在中间添加省略号
	 * @tparam StringType 字符串类型 (std::string, std::wstring等)
	 * @param data 输入数据
	 * @param max_length 最大长度（默认50）
	 * @return 处理后的字符串
	 */
	template<typename StringType = std::string, typename DataType>
	StringType LimitStringLength(const DataType& data, size_t max_length = 50)
	{
		StringType s;
		if constexpr (std::is_convertible_v<DataType, StringType>)
		{
			s = static_cast<StringType>(data);
		}
		else
		{
			if constexpr (std::is_same_v<StringType, std::string>)
				s = std::to_string(data);
			else if constexpr (std::is_same_v<StringType, std::wstring>)
				s = std::to_wstring(data);
			else
				s = StringType(data);
		}

		if (s.length() <= max_length)
		{
			return s;
		}
		else
		{
			StringType inside_str;
			if constexpr (std::is_same_v<StringType, std::wstring>)
				inside_str = L"\n...\n...\n";
			else
				inside_str = "\n...\n...\n";

			// 计算头尾部分的长度
			size_t head_length = max_length / 2;
			size_t tail_length = max_length - head_length - inside_str.length();

			// 截取头尾部分并连接
			return s.substr(0, head_length) + inside_str + s.substr(s.length() - tail_length);
		}
	}

	/**
	 * @brief 字符串填充对齐
	 * @tparam StringType 字符串类型
	 * @param data 输入数据
	 * @param max_length 目标长度（默认50）
	 * @param fill_char 填充字符（默认空格）
	 * @param side 对齐方式: "left"(左对齐)、"right"(右对齐)、"center"(居中，默认right）
	 * @return 填充后的字符串
	 */
	template<typename StringType = std::string, typename DataType>
	StringType FillString(
		const DataType& data,
		size_t max_length = 50,
		typename StringType::value_type fill_char = ' ',
		const char* side = "right")
	{
		StringType s;
		if constexpr (std::is_convertible_v<DataType, StringType>)
		{
			s = static_cast<StringType>(data);
		}
		else
		{
			if constexpr (std::is_same_v<StringType, std::string>)
				s = std::to_string(data);
			else if constexpr (std::is_same_v<StringType, std::wstring>)
				s = std::to_wstring(data);
			else
				s = StringType(data);
		}

		if (s.length() >= max_length)
		{
			return s;
		}
		else
		{
			size_t fill_count = max_length - s.length();

			if (strcmp(side, "left") == 0)
			{
				return s + StringType(fill_count, fill_char);
			}
			else if (strcmp(side, "right") == 0)
			{
				return StringType(fill_count, fill_char) + s;
			}
			else if (strcmp(side, "center") == 0)
			{
				size_t left = fill_count / 2;
				size_t right = fill_count - left;
				return StringType(left, fill_char) + s + StringType(right, fill_char);
			}
			else
			{
				throw std::invalid_argument("Unsupported side: must be 'left', 'right', or 'center'");
			}
		}
	}

	/**
	 * @brief 将字节数组转换为字符串数组
	 * @param lines 字节数组列表
	 * @return 字符串数组
	 */
	inline std::vector<std::string> Bytes2Strings(const std::vector<std::vector<uint8_t>>& lines)
	{
		std::vector<std::string> result;
		result.reserve(lines.size());

		for (const auto& line : lines)
		{
			result.emplace_back(line.begin(), line.end());
		}

		return result;
	}

	/**
	 * @brief 将字节数组转换为字符串数组（使用char*指针和长度）
	 * @param lines 字节数组列表
	 * @return 字符串数组
	 */
	inline std::vector<std::string> Bytes2Strings(const std::vector<std::pair<const char*, size_t>>& lines)
	{
		std::vector<std::string> result;
		result.reserve(lines.size());

		for (const auto& [ptr, len] : lines)
		{
			result.emplace_back(ptr, len);
		}

		return result;
	}

	/**
	 * @brief 将字节数组转换为单个字符串
	 * @param lines 字节数组列表
	 * @return 连接后的字符串
	 */
	inline std::string Bytes2String(const std::vector<std::vector<uint8_t>>& lines)
	{
		auto strings = Bytes2Strings(lines);
		std::string result;

		// 预先计算总长度
		size_t total_length = 0;
		for (const auto& str : strings)
		{
			total_length += str.length();
		}
		result.reserve(total_length);

		// 连接所有字符串
		for (const auto& str : strings)
		{
			result += str;
		}

		return result;
	}

	/**
	 * @brief 将字节数组转换为单个字符串（使用char*指针和长度）
	 * @param lines 字节数组列表
	 * @return 连接后的字符串
	 */
	inline std::string Bytes2String(const std::vector<std::pair<const char*, size_t>>& lines)
	{
		auto strings = Bytes2Strings(lines);
		std::string result;

		// 预先计算总长度
		size_t total_length = 0;
		for (const auto& str : strings)
		{
			total_length += str.length();
		}
		result.reserve(total_length);

		// 连接所有字符串
		for (const auto& str : strings)
		{
			result += str;
		}

		return result;
	}

	/**
	 * @brief 字符串操作类型枚举
	 */
	enum class StringOpType
	{
		Add,    // 添加操作
		Delete  // 删除操作
	};

	/**
	 * @brief 字符串操作结构
	 * 包含: (操作类型, 开始位置, 结束位置, 内容)
	 */
	struct StringOperation
	{
		StringOpType type;     // 操作类型
		size_t start;          // 开始位置
		size_t end;            // 结束位置
		std::string content;   // 操作内容

		StringOperation(StringOpType t, size_t s, size_t e, std::string c)
			: type(t), start(s), end(e), content(std::move(c)) {
		}
	};

	/**
	 * @brief 计算两个字符串的编辑距离和操作序列
	 * 使用LCS算法来找到最长公共子序列，然后基于LCS生成操作序列
	 * @param s1 源字符串
	 * @param s2 目标字符串
	 * @return (编辑距离, 操作序列)
	 */
	inline std::pair<int, std::vector<StringOperation>> GetEditorDistanceAndOperations(
		const std::string& s1,
		const std::string& s2)
	{
		size_t m = s1.length();
		size_t n = s2.length();

		// 使用LCS算法构建动态规划表
		std::vector<std::vector<int>> lcs(m + 1, std::vector<int>(n + 1, 0));

		// 构建LCS表
		for (size_t i = 1; i <= m; ++i)
		{
			for (size_t j = 1; j <= n; ++j)
			{
				if (s1[i - 1] == s2[j - 1])
				{
					lcs[i][j] = lcs[i - 1][j - 1] + 1;
				}
				else
				{
					lcs[i][j] = std::max(lcs[i - 1][j], lcs[i][j - 1]);
				}
			}
		}

		// 基于LCS生成操作序列
		std::vector<StringOperation> operations;
		size_t i = m, j = n;

		while (i > 0 || j > 0)
		{
			if (i > 0 && j > 0 && s1[i - 1] == s2[j - 1])
			{
				// 字符匹配，不需要操作
				--i;
				--j;
			}
			else if (j > 0 && (i == 0 || lcs[i][j - 1] >= lcs[i - 1][j]))
			{
				// 需要插入s2[j-1]
				size_t insert_pos = i;
				operations.insert(operations.begin(),
					StringOperation(StringOpType::Add, insert_pos, insert_pos, std::string(1, s2[j - 1])));
				--j;
			}
			else
			{
				// 需要删除s1[i-1]
				operations.insert(operations.begin(),
					StringOperation(StringOpType::Delete, i - 1, i, std::string(1, s1[i - 1])));
				--i;
			}
		}

		// 合并连续的操作
		std::vector<StringOperation> merged_operations;
		for (const auto& op : operations)
		{
			if (!merged_operations.empty() && merged_operations.back().type == op.type)
			{
				auto& last_op = merged_operations.back();
				if (op.type == StringOpType::Add && last_op.end == op.start)
				{
					// 合并连续的添加操作
					last_op.end = op.end;
					last_op.content += op.content;
				}
				else if (op.type == StringOpType::Delete && last_op.end == op.start)
				{
					// 合并连续的删除操作
					last_op.end = op.end;
					last_op.content += op.content;
				}
				else
				{
					merged_operations.push_back(op);
				}
			}
			else
			{
				merged_operations.push_back(op);
			}
		}

		// 计算编辑距离
		int edit_distance = static_cast<int>(m + n - 2 * lcs[m][n]);
		return { edit_distance, merged_operations };
	}

	/**
	 * @brief 构建行级LCS动态规划表
	 * @param lines1 源字符串行数组
	 * @param lines2 目标字符串行数组
	 * @return LCS动态规划表
	 */
	inline std::vector<std::vector<int>> _build_line_lcs(
		const std::vector<std::string>& lines1,
		const std::vector<std::string>& lines2)
	{
		size_t m = lines1.size();
		size_t n = lines2.size();
		std::vector<std::vector<int>> lcs(m + 1, std::vector<int>(n + 1, 0));

		// 使用哈希加速行比较
		std::vector<size_t> hash1, hash2;
		hash1.reserve(m);
		hash2.reserve(n);

		std::hash<std::string> hasher;
		for (const auto& line : lines1)
			hash1.push_back(hasher(line));
		for (const auto& line : lines2)
			hash2.push_back(hasher(line));

		for (size_t i = 1; i <= m; ++i)
		{
			for (size_t j = 1; j <= n; ++j)
			{
				if (hash1[i - 1] == hash2[j - 1] && lines1[i - 1] == lines2[j - 1])
				{
					lcs[i][j] = lcs[i - 1][j - 1] + 1;
				}
				else
				{
					lcs[i][j] = std::max(lcs[i - 1][j], lcs[i][j - 1]);
				}
			}
		}

		return lcs;
	}

	/**
	 * @brief 行级操作结构
	 */
	struct LineOperation
	{
		StringOpType type;              // 操作类型
		size_t start_line;              // 起始行号
		size_t end_line;                // 结束行号
		std::vector<std::string> lines; // 行内容列表

		LineOperation(StringOpType t, size_t s, size_t e, std::vector<std::string> l)
			: type(t), start_line(s), end_line(e), lines(std::move(l)) {
		}
	};

	/**
	 * @brief 从LCS表提取行级操作序列
	 * @param lines1 源字符串行数组
	 * @param lines2 目标字符串行数组
	 * @param lcs LCS动态规划表
	 * @return 行级操作序列
	 */
	inline std::vector<LineOperation> _extract_line_operations(
		const std::vector<std::string>& lines1,
		const std::vector<std::string>& lines2,
		const std::vector<std::vector<int>>& lcs)
	{
		std::vector<LineOperation> operations;
		size_t m = lines1.size();
		size_t n = lines2.size();
		size_t i = m, j = n;

		while (i > 0 || j > 0)
		{
			if (i > 0 && j > 0 && lines1[i - 1] == lines2[j - 1])
			{
				--i;
				--j;
			}
			else if (j > 0 && (i == 0 || lcs[i][j - 1] >= lcs[i - 1][j]))
			{
				std::vector<std::string> temp_lines = { lines2[j - 1] };
				operations.insert(operations.begin(),
					LineOperation(StringOpType::Add, i, i, std::move(temp_lines)));
				--j;
			}
			else
			{
				std::vector<std::string> temp_lines = { lines1[i - 1] };
				operations.insert(operations.begin(),
					LineOperation(StringOpType::Delete, i - 1, i, std::move(temp_lines)));
				--i;
			}
		}

		// 合并连续的同类行操作
		std::vector<LineOperation> merged;
		for (auto& op : operations)
		{
			if (!merged.empty() && merged.back().type == op.type && merged.back().end_line == op.start_line)
			{
				merged.back().end_line = op.end_line;
				merged.back().lines.insert(merged.back().lines.end(),
					op.lines.begin(), op.lines.end());
			}
			else
			{
				merged.push_back(std::move(op));
			}
		}

		return merged;
	}

	/**
	 * @brief 对小范围区域进行字符级LCS比较
	 * @param s1 源字符串
	 * @param s2 目标字符串
	 * @return 操作序列（相对于输入字符串的位置）
	 */
	inline std::vector<StringOperation> _char_diff_in_region(
		const std::string& s1,
		const std::string& s2)
	{
		size_t m = s1.length();
		size_t n = s2.length();

		// 快速路径
		if (m == 0 && n == 0)
			return {};
		if (m == 0)
			return { StringOperation(StringOpType::Add, 0, 0, s2) };
		if (n == 0)
			return { StringOperation(StringOpType::Delete, 0, m, s1) };
		if (s1 == s2)
			return {};

		// 字符级LCS
		std::vector<std::vector<int>> lcs(m + 1, std::vector<int>(n + 1, 0));

		for (size_t i = 1; i <= m; ++i)
		{
			for (size_t j = 1; j <= n; ++j)
			{
				if (s1[i - 1] == s2[j - 1])
				{
					lcs[i][j] = lcs[i - 1][j - 1] + 1;
				}
				else
				{
					lcs[i][j] = std::max(lcs[i - 1][j], lcs[i][j - 1]);
				}
			}
		}

		// 回溯生成操作
		std::vector<StringOperation> operations;
		size_t i = m, j = n;

		while (i > 0 || j > 0)
		{
			if (i > 0 && j > 0 && s1[i - 1] == s2[j - 1])
			{
				--i;
				--j;
			}
			else if (j > 0 && (i == 0 || lcs[i][j - 1] >= lcs[i - 1][j]))
			{
				operations.insert(operations.begin(),
					StringOperation(StringOpType::Add, i, i, std::string(1, s2[j - 1])));
				--j;
			}
			else
			{
				operations.insert(operations.begin(),
					StringOperation(StringOpType::Delete, i - 1, i, std::string(1, s1[i - 1])));
				--i;
			}
		}

		// 合并连续操作
		std::vector<StringOperation> merged;
		for (const auto& op : operations)
		{
			if (!merged.empty() && merged.back().type == op.type)
			{
				auto& last_op = merged.back();
				if (op.type == StringOpType::Add && last_op.end == op.start)
				{
					last_op.end = op.end;
					last_op.content += op.content;
				}
				else if (op.type == StringOpType::Delete && last_op.end == op.start)
				{
					last_op.end = op.end;
					last_op.content += op.content;
				}
				else
				{
					merged.push_back(op);
				}
			}
			else
			{
				merged.push_back(op);
			}
		}

		return merged;
	}

	/**
	 * @brief 分割字符串为行数组
	 * @param s 输入字符串
	 * @return 行数组
	 */
	inline std::vector<std::string> _split_lines(const std::string& s)
	{
		std::vector<std::string> lines;
		size_t start = 0;
		size_t pos = 0;

		while ((pos = s.find('\n', start)) != std::string::npos)
		{
			lines.push_back(s.substr(start, pos - start));
			start = pos + 1;
		}

		// 添加最后一行（即使为空）
		lines.push_back(s.substr(start));

		return lines;
	}

	/**
	 * @brief 连接字符串数组为单个字符串
	 * @param lines 字符串数组
	 * @param separator 分隔符（默认换行符）
	 * @return 连接后的字符串
	 */
	inline std::string _join_lines(const std::vector<std::string>& lines, const std::string& separator = "\n")
	{
		if (lines.empty())
			return "";

		std::string result = lines[0];
		for (size_t i = 1; i < lines.size(); ++i)
		{
			result += separator + lines[i];
		}
		return result;
	}

	/**
	 * @brief 计算两个字符串的差异操作序列（混合行级+字符级算法）
	 * 操作格式: (操作类型, 开始位置, 结束位置, 内容)
	 * 位置基于源字符串s1的字符偏移
	 * @param s1 源字符串
	 * @param s2 目标字符串
	 * @return 差异操作序列
	 */
	inline std::vector<StringOperation> GetDiffOperations(
		const std::string& s1,
		const std::string& s2)
	{
		// 快速路径
		if (s1 == s2)
			return {};
		if (s1.empty())
			return { StringOperation(StringOpType::Add, 0, 0, s2) };
		if (s2.empty())
			return { StringOperation(StringOpType::Delete, 0, s1.length(), s1) };

		// 阶段1: 分行并建立位置映射
		std::vector<std::string> lines1 = _split_lines(s1);
		std::vector<std::string> lines2 = _split_lines(s2);

		// 构建行号到字符位置的映射
		std::vector<size_t> line_offsets_s1 = { 0 };
		for (size_t i = 0; i < lines1.size() - 1; ++i)
		{
			line_offsets_s1.push_back(line_offsets_s1.back() + lines1[i].length() + 1); // +1 for '\n'
		}

		std::vector<size_t> line_offsets_s2 = { 0 };
		for (size_t i = 0; i < lines2.size() - 1; ++i)
		{
			line_offsets_s2.push_back(line_offsets_s2.back() + lines2[i].length() + 1);
		}

		// 阶段2: 行级LCS分析
		auto lcs = _build_line_lcs(lines1, lines2);
		auto line_operations = _extract_line_operations(lines1, lines2, lcs);

		// 阶段3: 转换为字符级操作
		std::vector<StringOperation> final_operations;

		for (const auto& line_op : line_operations)
		{
			if (line_op.type == StringOpType::Add)
			{
				// 添加操作: 在s1的start_line位置插入
				size_t char_pos = (line_op.start_line < line_offsets_s1.size())
					? line_offsets_s1[line_op.start_line]
					: s1.length();
				std::string content = _join_lines(line_op.lines);

				final_operations.emplace_back(StringOpType::Add, char_pos, char_pos, content);
			}
			else if (line_op.type == StringOpType::Delete)
			{
				// 删除操作: 删除s1的[start_line, end_line)行
				size_t char_start = line_offsets_s1[line_op.start_line];
				size_t char_end = (line_op.end_line < lines1.size())
					? line_offsets_s1[line_op.end_line]
					: s1.length();

				std::string content = _join_lines(line_op.lines);
				final_operations.emplace_back(StringOpType::Delete, char_start, char_end, content);
			}
		}

		// 阶段4: 对于连续的删除+添加，尝试字符级精细比较
		std::vector<StringOperation> optimized_operations;
		size_t i = 0;

		while (i < final_operations.size())
		{
			if (i + 1 < final_operations.size() &&
				final_operations[i].type == StringOpType::Delete &&
				final_operations[i + 1].type == StringOpType::Add &&
				final_operations[i].end == final_operations[i + 1].start)
			{
				// 这是一个修改操作，进行字符级细化
				const auto& del_op = final_operations[i];
				const auto& add_op = final_operations[i + 1];

				const std::string& old_text = del_op.content;
				const std::string& new_text = add_op.content;
				size_t base_pos = del_op.start;

				// 字符级比较
				auto char_ops = _char_diff_in_region(old_text, new_text);

				// 调整位置到全局坐标
				for (const auto& op : char_ops)
				{
					optimized_operations.emplace_back(
						op.type,
						base_pos + op.start,
						base_pos + op.end,
						op.content
					);
				}

				i += 2;
			}
			else
			{
				optimized_operations.push_back(final_operations[i]);
				++i;
			}
		}

		return optimized_operations;
	}
}

#endif // !Convention_Runtime_String_Hpp
