# CSoft Internship Project

## Project Overview

Welcome to the CSoft Internship Project! This solution is designed to provide hands-on experience with professional C++ development using MFC (Microsoft Foundation Classes) and follows the **Мulti-layered, component-based architectural pattern**.

## Architecture

This project implements a multi-layered architecture that separates concerns and promotes maintainable, scalable code. The solution follows industry best practices for enterprise-level C++ application development.

## Components

### ClientApp

**Type:** Main Application (.exe)  
**Technology:** MFC MDI (Multiple Document Interface)  
**Purpose:**
- Main entry point for the entire application
- Coordinates communication between all components
- Manages the overall application lifecycle

### Core.dll

**Type:** Utility Library  
**Purpose:**
- Contains features and utilities that enhance C++ language capabilities
- Provides reusable components applicable to all other application modules
- Serves as the foundation library for shared functionality

### Domain.dll

**Type:** Data Models Library  
**Purpose:**
- Contains structs (also referred to as database/disk or table structs)
- Defines business models and entities used throughout the application
- Represents the core data structures of the business domain

### Database.dll

**Type:** Data Access Layer  
**Purpose:**
- Handles all database-related operations
- Provides data persistence and retrieval functionality
- Abstracts database implementation details from business logic

### Application.dll

**Type:** Business Logic Layer  
**Purpose:**
- Contains all business logic and application workflows
- Implements core application functionality
- Processes business rules and application-specific operations

### Document.dll

**Type:** Presenter Layer
**Technology:** CDocument-derived classes  
**Purpose:**
- Acts as the coordinator between UI and application logic
- Manages the interaction between data (Model) and user interface (View)
- Contains document management and presentation logic

### UIView.dll

**Type:** UI Layer

**Technology:** CListView, CTreeView, and other MFC view classes  
**Purpose:**
- Handles data display and user interface rendering
- Contains various MFC-derived view classes for different display needs

### UIDialog.dll

**Type:** UI Layer 
**Technology:** CDialog-derived classes  
**Purpose:**
- Implements dialog-based user interface components
- Handles user interactions through dialog boxes and forms

## Architecture Principles

- **Separation of Concerns:** Each component has a specific responsibility
- **Modularity:** Components are designed to be loosely coupled and highly cohesive
- **Reusability:** Core utilities are shared across all components

## Technology Stack

- **Language:** C++
- **Framework:** Microsoft Foundation Classes (MFC)
- **Architecture:** Мulti-layered, component-based
- **Database:** olddb integration
- **Platform:** Windows

## Component Dependencies

- All components depend on Core.dll for shared utilities
- All components depend on Domain.dll for data models and entities
- UI components (UIView.dll, UIDialog.dll) interact through Document.dll
- Separete Business logic layer in Application.dll
- Data operations flow through Database.dll

## Learning Objectives

Through this project, you will gain experience with:

- Professional C++ development practices
- MFC application development
- Multi-project solution organization
- Component-based software design
- Database integration in C++ applications

## About the company

- **Name:** CSoft
- **Address:** Varna 9000, Bulgaria, 54 "8-th Primorski polk" blvd.
- **Website:** https://www.csoft.bg/