/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_codes.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 20:03:09 by jschroed          #+#    #+#             */
/*   Updated: 2024/05/25 12:37:21 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXIT_CODES_H
# define EXIT_CODES_H

// Command-related exit codes
# define EXIT_CMD_NOT_FOUND			127  // Command not found
# define EXIT_CMD_INV_USAGE			2  // Invalid usage of a built-in command
# define EXIT_CMD_EXEC_ERROR		126  // Command found but not executable

// Shell-specific exit codes
# define EXIT_INVALID_SYNTAX		2  // Invalid syntax
# define EXIT_OUT_OF_MEMORY			128  // Out of memory
# define EXIT_FILE_NOT_FOUND		2  // File not found
# define EXIT_PERMISSION_DENIED		13 // Permission denied
# define EXIT_NO_SUCH_USER			6  // No such user or login
# define EXIT_INVALID_REDIRECT		3   // Invalid redirection syntax
# define EXIT_AMBIGUOUS_REDIRECT   	4   // Ambiguous redirection
# define EXIT_VARIABLE_NOT_FOUND   	14  // Variable not found
# define EXIT_INVALID_EXPANSION    	15  // Invalid parameter expansion
# define EXIT_INVALID_ARITHMETIC   	16  // Invalid arithmetic expansion

// Input/Output error exit codes
# define EXIT_IO_ERROR				5  // Input/output error
# define EXIT_CANNOT_WRITE			28  // Write error (no space left on device)
# define EXIT_CANNOT_READ			29  // Read error

// Process management exit codes
# define EXIT_CHILD_PROCESS_FAILURE 10 // Child process failure
# define EXIT_FORK_FAILURE          11 // Fork failure
# define EXIT_PIPE_FAILURE          12 // Pipe failure

// Signal-related exit codes
# define EXIT_SIGINT				130  // Terminated by SIGINT (Ctrl+C)
# define EXIT_SIGQUIT				131  // Terminated by SIGQUIT (Ctrl+\)

// Command-related exit codes
# define EXIT_CMD_TOO_MANY_ARGS		7   // Too many args provided to a command
# define EXIT_CMD_TOO_FEW_ARGS		8   // Too few args provided to a command
# define EXIT_CMD_INVALID_ARG      	9   // Invalid args provided to a command

// Process management exit codes
# define EXIT_EXEC_FAILURE			17  // Execution failure (permission denied)
# define EXIT_INVALID_JOB_SPEC     	18  // Invalid job specification
# define EXIT_NO_SUCH_JOB          	19  // No such job

#endif // EXIT_CODES_H
